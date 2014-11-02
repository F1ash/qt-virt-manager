#include "generic_ethernet.h"

Generic_Ethernet::Generic_Ethernet(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    targetLabel = new QLabel("Target Device name:", this);
    scriptLabel = new QLabel("Script path:", this);
    target = new QLineEdit(this);
    target->setPlaceholderText("vnetN");
    script = new QLineEdit(this);
    script->setPlaceholderText("/etc/qemu-ifup");
    baseLayout = new QGridLayout();
    baseLayout->addWidget(targetLabel, 0, 0);
    baseLayout->addWidget(target, 0, 1);
    baseLayout->addWidget(scriptLabel, 1, 0);
    baseLayout->addWidget(script, 1, 1);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    addr = new DeviceAddress(this);
    int idx = addr->type->findData(
                "pci",
                Qt::UserRole,
                Qt::MatchContains);
    addr->type->setCurrentIndex( (idx<0)? 0:idx );
    addr->type->setEnabled(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(addr);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    // dataChanged connects
    connect(target, SIGNAL(textEdited(QString)),
            this, SIGNAL(dataChanged()));
    connect(script, SIGNAL(textEdited(QString)),
            this, SIGNAL(dataChanged()));
    connect(addr, SIGNAL(dataChanged()),
            this, SIGNAL(dataChanged()));
}

/* public slots */
QDomDocument Generic_Ethernet::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _target, _script, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("interface");
    QString _dev, _scr;
    _dev = target->text();
    _scr = script->text();
    if ( !_dev.isEmpty() ) {
        _target = doc.createElement("target");
        _target.setAttribute("dev", _dev);
        _devDesc.appendChild(_target);
    };
    if ( !_scr.isEmpty() ) {
        _script = doc.createElement("script");
        _script.setAttribute("path", _scr);
        _devDesc.appendChild(_script);
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
    _devDesc.setAttribute("type", "ethernet");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void Generic_Ethernet::setDataDescription(QString &xmlDesc)
{
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _target, _script, _addr;
    _device = doc.firstChildElement("device")
            .firstChildElement("interface");
    _target = _device.firstChildElement("target");
    _script = _device.firstChildElement("script");
    QString _attr;
    _attr = _target.attribute("dev");
    target->setText(_attr);
    _attr = _script.attribute("path");
    script->setText(_attr);
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
        wdg->function->setValue( _addr.attribute("function").toInt() );
    };
}
