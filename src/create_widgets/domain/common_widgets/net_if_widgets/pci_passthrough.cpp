#include "pci_passthrough.h"

PCI_Passthrough::PCI_Passthrough(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    attentionIcon = new QLabel(this);
    attentionIcon->setPixmap(QIcon::fromTheme("dialog-warning").pixmap(32));
    attention = new QLabel("WARNING: only for SR-IOV\n(Single Root I/O Virtualization)", this);
    driverLabel = new QLabel("Driver:", this);
    driver = new QComboBox(this);
    connType = QString(virConnectGetType(currWorkConnect)).toLower();
    if ( connType=="lxc" ) {
        driver->addItem("VFIO");
    } else if ( connType=="qemu" ) {
        driver->addItem("VFIO");
        driver->addItem("KVM");
    };
    macLabel = new QLabel("MAC:", this);
    mac = new QLineEdit(this);
    mac->setPlaceholderText("11:22:33:44:55:66");
    pciAddrLabel = new QLabel("PCI Device Address:", this);
    addr = new PciAddr(this);
    baseLayout = new QGridLayout();
    baseLayout->addWidget(attentionIcon, 0, 0);
    baseLayout->addWidget(attention, 0, 1);
    baseLayout->addWidget(driverLabel, 1, 0);
    baseLayout->addWidget(driver, 1, 1);
    baseLayout->addWidget(macLabel, 2, 0);
    baseLayout->addWidget(mac, 2, 1);
    baseLayout->addWidget(pciAddrLabel, 3, 0);
    baseLayout->addWidget(addr, 4, 0, 5, 2, Qt::AlignTop);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    virtPort = new VirtualPort(this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(virtPort);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    virtPort->type->setCurrentIndex( virtPort->type->findText("802.1Qbh") );
}

/* public slots */
QDomDocument PCI_Passthrough::getDataDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _address, _mac, _driver,
            _virtualport, _parameters, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("interface");
    _driver = doc.createElement("driver");
    _driver.setAttribute("name", driver->currentText().toLower());
    _devDesc.appendChild(_driver);
    _source = doc.createElement("source");

    AttrList l = addr->getAttrList();
    _address = doc.createElement("address");
    foreach (QString key, l.keys()) {
        if ( !key.isEmpty() )
        _address.setAttribute(key, l.value(key));
    };
    _address.setAttribute("type", "pci");
    _source.appendChild(_address);

    _devDesc.appendChild(_source);

    ParameterList p = virtPort->getParameterList();
    if ( !p.isEmpty() ) {
        _virtualport = doc.createElement("virtualport");
        _parameters = doc.createElement("parameters");
        foreach (QString key, p.keys()) {
            if ( !key.isEmpty() ) {
                if ( key=="type" ) {
                    _virtualport.setAttribute(key, p.value(key));
                } else
                    _parameters.setAttribute(key, p.value(key));
            };
            _virtualport.appendChild(_parameters);
        };
        _devDesc.appendChild(_virtualport);
    };

    if ( !mac->text().isEmpty() ) {
        _mac = doc.createElement("mac");
        _mac.setAttribute("address", mac->text());
        _devDesc.appendChild(_mac);
    };
    _devDesc.setAttribute("type", "hostdev");
    _devDesc.setAttribute("managed", "yes");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
