#include "_device.h"

#define DEVICE_TYPES QStringList()\
    <<"Disk"<<"Floppy"<<"CDROM"<<"LUN"

_Device::_Device(QWidget *parent) :
    _Changed(parent)
{
    devTypeLabel = new QLabel(tr("Device type:"), this);
    devType = new QComboBox(this);
    devType->addItems(DEVICE_TYPES);
    devType->setItemIcon(0, QIcon::fromTheme("drive-harddisk"));
    devType->setItemIcon(1, QIcon::fromTheme("media-floppy"));
    devType->setItemIcon(2, QIcon::fromTheme("drive-optical"));
    devType->setItemIcon(3, QIcon::fromTheme("network-server-database"));
    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(devTypeLabel);
    commonLayout->addWidget(devType);
    setLayout(commonLayout);
    //  dataChanged connections
    connect(devType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
}

/* public slots */
QString _Device::getDeviceType() const
{
    return devType->currentText().toLower();
}
void _Device::setDeviceType(QString &_type)
{
    int idx = devType->findText(
                _type, Qt::MatchContains);
    devType->setCurrentIndex( (idx<0)? 0:idx );
}
