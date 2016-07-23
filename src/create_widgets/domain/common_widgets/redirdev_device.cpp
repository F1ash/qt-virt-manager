#include "redirdev_device.h"

/*
 * http://libvirt.org/formatdomain.html#elementsRedir
 */

RedirDevDevice::RedirDevDevice(
        QWidget *parent, virConnectPtr *connPtrPtr) :
    _QWidget(parent, connPtrPtr)
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

    address = new DeviceAddress(this);
    int idx = address->type->findData(
                "usb",
                Qt::UserRole,
                Qt::MatchContains);
    address->type->setCurrentIndex( (idx<0)? 0:idx );
    address->type->setEnabled(false);
    address->setCurrentAddrWidget(idx);

    filter = new RedirFilter(this, "Use filter");

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(type);
    commonLayout->addWidget(source);
    commonLayout->addWidget(address);
    commonLayout->addWidget(filter);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(typeChanged(int)));
}

/* public slots */
QDomDocument RedirDevDevice::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _address, _source, _device, _devDesc, _redirFilter;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("redirdev");
    _redirFilter = doc.createElement("redirfilter");

    QString _type = type->itemData(type->currentIndex(), Qt::UserRole).toString();
    if ( _type == "tcp" ) {
        _source = doc.createElement("source");
        _source.setAttribute("mode", "connect");
        _source.setAttribute("host", host->text());
        _source.setAttribute("service", port->text());
        _devDesc.appendChild(_source);
    };
    AttrList l = address->getAttrList();
    if ( address->use->isChecked() && !l.isEmpty() ) {
        _address = doc.createElement("address");
        foreach (QString key, l.keys()) {
            if ( !key.isEmpty() )
                _address.setAttribute(key, l.value(key));
        };
        _devDesc.appendChild(_address);
    };
    _devDesc.setAttribute("type", _type );
    /*
     * USB device redirection through a character device is supported.
     * bus must be "usb" for a USB device.
     */
    _devDesc.setAttribute("bus", "usb");
    _device.appendChild(_devDesc);
    QStringList _filters = filter->getFiltersList();
    if ( filter->isUsed() && !_filters.isEmpty() ) {
        foreach (QString _f, _filters) {
            if ( !_f.isEmpty() ) {
                QString _class, _vendor, _product, _version, _allow;
                QStringList _split = _f.split(":");
                if ( _split.count()>4 ) {
                    _class = _split.at(0);
                    _vendor = _split.at(1);
                    _product = _split.at(2);
                    _version = _split.at(3);
                    _allow = _split.at(4);
                    QDomElement _usbDev = doc.createElement("usbdev");
                    _usbDev.setAttribute("class", _class);
                    _usbDev.setAttribute("vendor", _vendor);
                    _usbDev.setAttribute("product", _product);
                    _usbDev.setAttribute("version", _version);
                    _usbDev.setAttribute("allow", _allow );
                    _redirFilter.appendChild(_usbDev);
                };
            };
        };
    };
    if ( !_redirFilter.childNodes().isEmpty() )
        _device.appendChild(_redirFilter);
    doc.appendChild(_device);
    return doc;
}

/* private slots */
void RedirDevDevice::typeChanged(int i)
{
    QString _type = type->itemData(i, Qt::UserRole).toString();
    source->setVisible( _type == "tcp" );
}
