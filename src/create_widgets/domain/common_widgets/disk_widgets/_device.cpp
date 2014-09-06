#include "_device.h"

#define DEVICE_TYPES QStringList()\
    <<"Disk"<<"Floppy"<<"CDROM"<<"LUN"

_Device::_Device(QWidget *parent) :
    QWidget(parent)
{
    devTypeLabel = new QLabel("Device type:", this);
    devType = new QComboBox(this);
    devType->addItems(DEVICE_TYPES);
    devType->setItemIcon(0, QIcon::fromTheme("drive-harddisk"));
    devType->setItemIcon(1, QIcon::fromTheme("media-floppy"));
    devType->setItemIcon(2, QIcon::fromTheme("drive-optical"));
    devType->setItemIcon(3, QIcon::fromTheme("network-server"));
    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(devTypeLabel);
    commonLayout->addWidget(devType);
    setLayout(commonLayout);
}

/* public slots */
QString _Device::getDeviceType() const
{
    return devType->currentText().toLower();
}
