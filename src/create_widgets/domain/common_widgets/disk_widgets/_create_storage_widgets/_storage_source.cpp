#include "_storage_source.h"

_Storage_Source::_Storage_Source(QWidget *parent) :
    QWidget(parent)
{
    deviceLabel = new QLabel("Device:", this);
    deviceLabel->setVisible(false);
    dirLabel = new QLabel("Dir:", this);
    dirLabel->setVisible(false);
    hostLabel = new QLabel("Host:", this);
    hostLabel->setVisible(false);
    namedLabel = new QLabel("Named:", this);
    namedLabel->setVisible(false);
    formatLabel = new QLabel("Format:", this);
    formatLabel->setVisible(false);
    vendorLabel = new QLabel("Vendor:", this);
    vendorLabel->setVisible(false);
    productLabel = new QLabel("Product:", this);
    productLabel->setVisible(false);
    device = new QLineEdit(this);
    device->setVisible(false);
    dir = new QLineEdit(this);
    dir->setVisible(false);
    host = new QLineEdit(this);
    host->setVisible(false);
    named = new QLineEdit(this);
    named->setVisible(false);
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
    auth = new _Storage_Auth(this);
    auth->setVisible(false);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(new QLabel("<b>Source</b>"), 0, 0);
    commonLayout->addWidget(deviceLabel, 1, 0);
    commonLayout->addWidget(device, 1, 1);
    commonLayout->addWidget(dirLabel, 2, 0);
    commonLayout->addWidget(dir, 2, 1);
    commonLayout->addWidget(adapterLabel, 3, 0);
    commonLayout->addWidget(adapter, 3, 1);
    commonLayout->addWidget(hostLabel, 4, 0);
    commonLayout->addWidget(host, 4, 1);
    commonLayout->addWidget(authLabel, 5, 0);
    commonLayout->addWidget(auth, 5, 1);
    commonLayout->addWidget(namedLabel, 6, 0);
    commonLayout->addWidget(named, 6, 1);
    commonLayout->addWidget(formatLabel, 7, 0);
    commonLayout->addWidget(format, 7, 1);
    commonLayout->addWidget(vendorLabel, 8, 0);
    commonLayout->addWidget(vendor, 8, 1);
    commonLayout->addWidget(productLabel, 9, 0);
    commonLayout->addWidget(product, 9, 1);
    setLayout(commonLayout);
}
