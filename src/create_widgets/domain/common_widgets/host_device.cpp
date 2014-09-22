#include "host_device.h"

/*
 * http://libvirt.org/formatdomain.html#elementsHostDev
 */

HostDevice::HostDevice(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    info = new QStackedWidget(this);
    info->addWidget(new USB_Host_Device(this, currWorkConnect));
    info->addWidget(new PCI_Host_Device(this, currWorkConnect));
    info->addWidget(new SCSI_Host_Device(this, currWorkConnect));
    info->addWidget(new BCh_Host_Device(this, currWorkConnect));
    type = new QComboBox(this);
    type->addItem("USB Host Device");
    type->addItem("PCI Host Device");
    type->addItem("SCSI Host Device");
    type->addItem("Block/Char Host Device");
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(type);
    commonLayout->addWidget(info);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(type, SIGNAL(currentIndexChanged(int)),
            info, SLOT(setCurrentIndex(int)));
}

/* public slots */
QDomDocument HostDevice::getDevDocument() const
{
    _QWidget *wdg = static_cast<_QWidget*>(info->currentWidget());
    return wdg->getDevDocument();
}
