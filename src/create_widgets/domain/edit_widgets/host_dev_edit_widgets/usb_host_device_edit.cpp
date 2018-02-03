#include "usb_host_device_edit.h"

USB_Host_Device_Edit::USB_Host_Device_Edit(QWidget *parent) :
    _QWidget(parent)
{
    vendorLabel = new QLabel(tr("Vendor:"), this);
    productLabel = new QLabel(tr("Product:"), this);
    vendor = new QLineEdit(this);
    product = new QLineEdit(this);
    startupPolicy = new _StartupPolicy(this);
    baseLayout = new QGridLayout();
    baseLayout->addWidget(vendorLabel, 0, 0);
    baseLayout->addWidget(vendor, 0, 1);
    baseLayout->addWidget(productLabel, 1, 0);
    baseLayout->addWidget(product, 1, 1);
    baseLayout->addWidget(startupPolicy, 2, 0, 3, 2);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    addr = new DeviceAddress(this);
    int idx = addr->type->findData(
                "usb", Qt::UserRole, Qt::MatchExactly);
    addr->type->setCurrentIndex( (idx<0)? 0:idx );
    addr->type->setEnabled(false);
    addr->setCurrentAddrWidget(idx);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(addr);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    // dataChanged signals
    connect(vendor, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(product, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(startupPolicy, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument USB_Host_Device_Edit::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _source, _product, _vendor, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("hostdev");
    _source = doc.createElement("source");
    _vendor = doc.createElement("vendor");
    _vendor.setAttribute("id", vendor->text());
    _product = doc.createElement("product");
    _product.setAttribute("id", product->text());
    _source.appendChild(_vendor);
    _source.appendChild(_product);
    if ( startupPolicy->isUsed() )
        _source.setAttribute(
                    "startupPolicy",
                    startupPolicy->getStartupPolicy());
    _devDesc.appendChild(_source);
    AttrList l = addr->getAttrList();
    if ( addr->use->isChecked() && !l.isEmpty() ) {
        QDomElement _address = doc.createElement("address");
        foreach (QString key, l.keys()) {
            if ( !key.isEmpty() )
                _address.setAttribute(key, l.value(key));
        };
        _devDesc.appendChild(_address);
    };
    _devDesc.setAttribute("type", "usb");
    _devDesc.setAttribute("mode", "subsystem");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void USB_Host_Device_Edit::setDataDescription(const QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _device, _source, _vendor, _product, _addr;
    _device = doc.firstChildElement("device")
            .firstChildElement("hostdev");
    _source = _device.firstChildElement("source");
    QString _startupPolicy = _source.attribute("startupPolicy");
    startupPolicy->setUsage(!_startupPolicy.isEmpty());
    int idx = startupPolicy->findPolicyIndex(_startupPolicy);
    startupPolicy->setPolicyIndex( (idx<0)? 0:idx );
    _vendor = _source.firstChildElement("vendor");
    _product = _source.firstChildElement("product");
    vendor->setText(_vendor.attribute("id"));
    product->setText(_product.attribute("id"));
    _addr = _device.firstChildElement("address");
    addr->use->setChecked(!_addr.isNull());
    if ( !_addr.isNull() ) {
        USBAddr *wdg = static_cast<USBAddr*>( addr->getCurrentAddrWidget() );
        if ( wdg!=nullptr ) {
            wdg->bus->setText( _addr.attribute("bus") );
            wdg->port->setText( _addr.attribute("port") );
        };
    };
}
