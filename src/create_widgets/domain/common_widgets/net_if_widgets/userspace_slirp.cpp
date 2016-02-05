#include "userspace_slirp.h"

#define INFO QString("\n\
Provides a virtual LAN with NAT to the outside world.\n\
The virtual network has DHCP & DNS services and \n\
will give the guest VM addresses starting from 10.0.2.15.\n\
The default router will be 10.0.2.2 and the DNS server\n\
will be 10.0.2.3. This networking is the only option\n\
for unprivileged users who need their VMs\n\
to have outgoing access. ")

Userspace_SLIRP::Userspace_SLIRP(
        QWidget *parent, virConnectPtr *connPtrPtr) :
    _QWidget(parent, connPtrPtr)
{
    mac = new MAC_Address(this);
    infoIcon = new QLabel(this);
    infoIcon->setPixmap(QIcon::fromTheme("dialog-warning")
                        .pixmap(this->fontInfo().pixelSize()));
    info = new QLabel(this);
    info->setText(INFO);
    addr = new DeviceAddress(this);
    int idx = addr->type->findData(
                "pci",
                Qt::UserRole,
                Qt::MatchContains);
    addr->type->setCurrentIndex( (idx<0)? 0:idx );
    addr->type->setEnabled(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(mac);
    commonLayout->addWidget(infoIcon);
    commonLayout->addWidget(info);
    commonLayout->addWidget(addr);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    // dataChanged connects
    connect(mac, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument Userspace_SLIRP::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _mac, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("interface");
    if ( mac->isUsed() ) {
        _mac = doc.createElement("mac");
        _mac.setAttribute("address", mac->getMACAddress());
        _devDesc.appendChild(_mac);
    };
    AttrList _l = addr->getAttrList();
    if ( !_l.isEmpty() ) {
        QDomElement _address = doc.createElement("address");
        foreach (QString key, _l.keys()) {
            if ( !key.isEmpty() )
            _address.setAttribute(key, _l.value(key));
        };
        _devDesc.appendChild(_address);
    };
    _devDesc.setAttribute("type", "user");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void Userspace_SLIRP::setDataDescription(QString &xmlDesc)
{
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _mac, _addr;
    _device = doc.firstChildElement("device")
            .firstChildElement("interface");
    _mac = _device.firstChildElement("mac");
    QString _attr;
    mac->setUsage(!_mac.isNull());
    if ( !_mac.isNull() ) {
        _attr = _mac.attribute("address");
        mac->setMACAddress(_attr);
    };
    _addr = _device.firstChildElement("address");
    addr->use->setChecked( !_addr.isNull() );
    if ( !_addr.isNull() ) {
        int idx = addr->type->findData(
                    "pci",
                    Qt::UserRole,
                    Qt::MatchContains);
        addr->type->setCurrentIndex( (idx<0)? 0:idx );
        addr->type->setEnabled(false);
        PciAddr *wdg = static_cast<PciAddr*>(addr->getCurrentAddrWidget());
        wdg->domain->setText( _addr.attribute("domain") );
        wdg->bus->setText( _addr.attribute("bus") );
        wdg->slot->setText( _addr.attribute("slot") );
        wdg->function->setValue( _addr.attribute("function")
                                 .split("x").last().toInt() );
        if ( _addr.hasAttribute("multifunction") ) {
            wdg->multifunction->setEnabled(true);
            wdg->multifunction->setChecked(
                        _addr.attribute("multifunction")=="on" );
        };
    };
}
