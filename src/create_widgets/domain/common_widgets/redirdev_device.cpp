#include "redirdev_device.h"

/*
 * http://libvirt.org/formatdomain.html#elementsRedir
 */

RedirDevDevice::RedirDevDevice(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    type = new QComboBox(this);
    type->addItem("TCP channel", "tcp");
    type->addItem("Spice channel", "spicevmc");

    hostLabel = new QLabel("Host:", this);
    portLabel = new QLabel("Port", this);
    host = new QLineEdit(this);
    port = new QSpinBox(this);
    port->setRange(10, 65535);
    port->setValue(4000);
    sourceLayout = new QHBoxLayout(this);
    sourceLayout->addWidget(hostLabel);
    sourceLayout->addWidget(host);
    sourceLayout->addWidget(portLabel);
    sourceLayout->addWidget(port);
    source = new QWidget(this);
    source->setLayout(sourceLayout);

    useAddress = new QCheckBox("Use address:", this);
    address = new USBAddr(this);
    address->setVisible(false);

    // TODO: implement filter
    filtered = new QCheckBox("Use filter:", this);
    filter = new QListWidget(this);
    filter->setVisible(false);
    filter->addItem("WARNING:\nDon't implemented");
    filter->item(0)->setIcon(QIcon::fromTheme("dialog-warning"));

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(type);
    commonLayout->addWidget(source);
    commonLayout->addWidget(useAddress);
    commonLayout->addWidget(address);
    commonLayout->addWidget(filtered);
    commonLayout->addWidget(filter);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(typeChanged(int)));
    typeChanged(0);
    connect(useAddress, SIGNAL(toggled(bool)),
            address, SLOT(setVisible(bool)));
    connect(filtered, SIGNAL(toggled(bool)),
            filter, SLOT(setVisible(bool)));
}

/* public slots */
QDomDocument RedirDevDevice::getDevDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _address, _source, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("redirdev");

    QString _type = type->itemData(type->currentIndex(), Qt::UserRole).toString();
    if ( _type == "tcp" ) {
        _source = doc.createElement("source");
        _source.setAttribute("mode", "connect");
        _source.setAttribute("host", host->text());
        _source.setAttribute("service", port->text());
        _devDesc.appendChild(_source);
    };
    AttrList l = address->getAttrList();
    if ( useAddress->isChecked() && !l.isEmpty() ) {
        _address = doc.createElement("address");
        foreach (QString key, l.keys()) {
            if ( !key.isEmpty() )
                _address.setAttribute(key, l.value(key));
        };
        _address.setAttribute("type", "usb");
        _devDesc.appendChild(_address);
    };
    _devDesc.setAttribute("type", _type );
    /*
     * USB device redirection through a character device is supported.
     * bus must be "usb" for a USB device.
     */
    _devDesc.setAttribute("bus", "usb");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}

/* private slots */
void RedirDevDevice::typeChanged(int i)
{
    QString _type = type->itemData(i, Qt::UserRole).toString();
    source->setVisible( _type == "tcp" );
}
