#include "host_device.h"

/*
 * http://libvirt.org/formatdomain.html#elementsHostDev
 */

HostDevice::HostDevice(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    type = new QComboBox(this);
    type->addItem("USB Host Device", "usb");
    type->addItem("PCI Host Device", "pci");
    type->addItem("SCSI Host Device", "scsi");
    type->addItem("Block/Char Host Device", "b|ch");
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(type);
    //commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(typeChanged(int)));
    typeChanged(0);
}

/* public slots */
QDomDocument HostDevice::getDevDocument() const
{
    return info->getDevDocument();
}

/* private slots */
void HostDevice::typeChanged(int i)
{
    QString _type = type->itemData(i, Qt::UserRole).toString();
    if ( info!=NULL ) {
        commonLayout->removeWidget(info);
        delete info;
        info = NULL;
    };
    if ( _type == "usb" ) {
        info = new USB_Host_Device(this, currWorkConnect);
    } else if ( _type == "pci" ) {
        info = new PCI_Host_Device(this, currWorkConnect);
    } else if ( _type == "scsi" ) {
        info = new SCSI_Host_Device(this, currWorkConnect);
    } else if ( _type == "b|ch" ) {
        info = new BCh_Host_Device(this, currWorkConnect);
    } else info = new _QWidget(this);
    commonLayout->insertWidget(1, info, -1);
}
