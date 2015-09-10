#include "host_device.h"

/*
 * http://libvirt.org/formatdomain.html#elementsHostDev
 */

#define QEMU_DEV_LIST QStringList()\
    <<"USB"<<"PCI"<<"SCSI"

#define LXC_DEV_LIST QStringList()\
    <<"USB"<<"Block/Char"

HostDevice::HostDevice(QWidget *parent, virConnectPtr *connPtr) :
    _QWidget(parent, connPtr)
{
    type = new QComboBox(this);
    info = new QStackedWidget(this);
    QString connType = QString(virConnectGetType(*currConnPtr)).toLower();
    if ( connType=="qemu" ) {
        type->addItems(QEMU_DEV_LIST);
        info->addWidget(new USB_Host_Device(this, currConnPtr));
        info->addWidget(new PCI_Host_Device(this, currConnPtr));
        info->addWidget(new SCSI_Host_Device(this, currConnPtr));
    } else if ( connType=="lxc" ) {
        type->addItems(LXC_DEV_LIST);
        info->addWidget(new USB_Host_Device(this, currConnPtr));
        info->addWidget(new BCh_Host_Device(this, currConnPtr));
    };
    /*
    info->addWidget(new USB_Host_Device(this, currConnPtr));
    info->addWidget(new PCI_Host_Device(this, currConnPtr));
    info->addWidget(new SCSI_Host_Device(this, currConnPtr));
    info->addWidget(new BCh_Host_Device(this, currConnPtr));
    type->addItem("USB Host Device");
    type->addItem("PCI Host Device");
    type->addItem("SCSI Host Device");
    type->addItem("Block/Char Host Device");
    */
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(type);
    commonLayout->addWidget(info);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(type, SIGNAL(currentIndexChanged(int)),
            info, SLOT(setCurrentIndex(int)));
}

/* public slots */
QDomDocument HostDevice::getDataDocument() const
{
    _QWidget *wdg = static_cast<_QWidget*>(info->currentWidget());
    return wdg->getDataDocument();
}
