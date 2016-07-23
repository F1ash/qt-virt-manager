#include "_storage_source.h"

_Storage_Source::_Storage_Source(QWidget *parent, virConnectPtr *connPtrPtr) :
    QWidget(parent)
{
    namedLabel = new QLabel("Name:", this);
    namedLabel->setVisible(false);
    hostLabel = new QLabel("Host:", this);
    hostLabel->setVisible(false);
    deviceLabel = new QLabel("Device:", this);
    deviceLabel->setVisible(false);
    dirLabel = new QLabel("Dir:", this);
    dirLabel->setVisible(false);
    formatLabel = new QLabel("Format:", this);
    formatLabel->setVisible(false);
    vendorLabel = new QLabel("Vendor:", this);
    vendorLabel->setVisible(false);
    productLabel = new QLabel("Product:", this);
    productLabel->setVisible(false);
    named = new QLineEdit(this);
    named->setPlaceholderText("a named element");
    named->setVisible(false);
    host = new _Hosts(this, "Use Hosts");
    host->setVisible(false);
    device = new _BlockDevices(this, "Block devices");
    device->setVisible(false);
    dir = new QLineEdit(this);
    dir->setVisible(false);
    format = new QComboBox(this);
    format->setVisible(false);
    vendor = new QLineEdit(this);
    vendor->setVisible(false);
    product = new QLineEdit(this);
    product->setVisible(false);
    adapterLabel = new QLabel("Adapter:", this);
    adapterLabel->setVisible(false);
    adapter = new _Adapter(this);
    adapter->setVisible(false);
    authLabel = new QLabel("Auth:", this);
    authLabel->setVisible(false);
    auth = new _Storage_Auth(this, connPtrPtr);
    auth->setVisible(false);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(new QLabel("<b>Source</b>"), 0, 0);
    commonLayout->addWidget(namedLabel, 1, 0);
    commonLayout->addWidget(named, 1, 1);
    commonLayout->addWidget(hostLabel, 2, 0);
    commonLayout->addWidget(host, 2, 1);
    commonLayout->addWidget(deviceLabel, 3, 0);
    commonLayout->addWidget(device, 3, 1);
    commonLayout->addWidget(dirLabel, 4, 0);
    commonLayout->addWidget(dir, 4, 1);
    commonLayout->addWidget(adapterLabel, 5, 0);
    commonLayout->addWidget(adapter, 5, 1);
    commonLayout->addWidget(authLabel, 6, 0);
    commonLayout->addWidget(auth, 6, 1);
    commonLayout->addWidget(formatLabel, 7, 0);
    commonLayout->addWidget(format, 7, 1);
    commonLayout->addWidget(vendorLabel, 8, 0);
    commonLayout->addWidget(vendor, 8, 1);
    commonLayout->addWidget(productLabel, 9, 0);
    commonLayout->addWidget(product, 9, 1);
    setLayout(commonLayout);
    setAutoFillBackground(true);
}
