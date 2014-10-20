#include "bch_host_device.h"

BCh_Host_Device::BCh_Host_Device(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    typeLabel = new QLabel("Type:", this);
    deviceLabel = new QLabel("Device:", this);
    type = new QComboBox(this);
    type->addItem("A block device", "storage");
    type->addItem("A character device", "misc");
    type->addItem("A host network interface", "net");
    device = new QLineEdit(this);
    device->setPlaceholderText("/dev/sdf1");
    info = new QLabel("The device as seen from the host", this);
    attention = new QLabel(this);
    attention->setPixmap(QIcon::fromTheme("dialog-warning").pixmap(32));
    commonLayout = new QGridLayout();
    commonLayout->addWidget(typeLabel, 0, 0);
    commonLayout->addWidget(type, 0, 1);
    commonLayout->addWidget(deviceLabel, 1, 0);
    commonLayout->addWidget(device, 1, 1);
    commonLayout->addWidget(attention, 2, 0, Qt::AlignTop);
    commonLayout->addWidget(info, 2, 1, Qt::AlignTop);
    setLayout(commonLayout);
    connect(type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(typeChanged(int)));
}

/* public slots */
QDomDocument BCh_Host_Device::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _source, _dev, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("hostdev");
    _source = doc.createElement("source");
    switch (type->currentIndex()) {
    case 0:
        _dev = doc.createElement("block");
        break;
    case 1:
        _dev = doc.createElement("char");
        break;
    case 2:
        _dev = doc.createElement("interface");
        break;
    default:
        _dev = doc.createElement("block");
        break;
    };
    QDomText data = doc.createTextNode(device->text());
    _dev.appendChild(data);
    _source.appendChild(_dev);
    _devDesc.appendChild(_source);
    _device.appendChild(_devDesc);
    _devDesc.setAttribute("type", type->itemData(type->currentIndex(), Qt::UserRole).toString());
    _devDesc.setAttribute("mode", "capabilities");
    doc.appendChild(_device);
    return doc;
}

/* private slots */
void BCh_Host_Device::typeChanged(int i)
{
    device->clear();
    switch (i) {
    case 0:
        device->setPlaceholderText("/dev/sdf1");
        break;
    case 1:
        device->setPlaceholderText("/dev/input/event3");
        break;
    case 2:
        device->setPlaceholderText("eth0");
        break;
    default:
        device->setPlaceholderText("The device as seen from the host");
        break;
    }
}
