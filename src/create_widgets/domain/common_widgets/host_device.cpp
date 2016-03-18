#include "host_device.h"

/*
 * http://libvirt.org/formatdomain.html#elementsHostDev
 */

#define QEMU_DEV_LIST QStringList()\
    <<"USB"<<"PCI"<<"SCSI"

#define LXC_DEV_LIST QStringList()\
    <<"USB"<<"Block/Char"

HostDevice::HostDevice(QWidget *parent, virConnectPtr *connPtrPtr) :
    _QWidget(parent, connPtrPtr)
{
    type = new QComboBox(this);
    info = new QStackedWidget(this);
    QString connType;
    if ( nullptr!=ptr_ConnPtr && nullptr!=*ptr_ConnPtr ) {
        connType = QString(virConnectGetType(*ptr_ConnPtr)).toLower();
    } else
        emit ptrIsNull();
    if ( connType=="qemu" ) {
        type->addItems(QEMU_DEV_LIST);
        info->addWidget(new USB_Host_Device(this, ptr_ConnPtr));
        info->addWidget(new PCI_Host_Device(this, ptr_ConnPtr));
        info->addWidget(new SCSI_Host_Device(this, ptr_ConnPtr));
    } else if ( connType=="lxc" ) {
        type->addItems(LXC_DEV_LIST);
        info->addWidget(new USB_Host_Device(this, ptr_ConnPtr));
        info->addWidget(new BCh_Host_Device(this, ptr_ConnPtr));
    };
    /*
    info->addWidget(new USB_Host_Device(this, ptr_ConnPtr));
    info->addWidget(new PCI_Host_Device(this, ptr_ConnPtr));
    info->addWidget(new SCSI_Host_Device(this, ptr_ConnPtr));
    info->addWidget(new BCh_Host_Device(this, ptr_ConnPtr));
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
    for(uint i=0; i<info->count(); ++i) {
        _QWidget *wdg = static_cast<_QWidget*>(
                    info->widget(i));
        if ( wdg!=nullptr ) {
            connect(wdg, SIGNAL(complete()),
                    this, SLOT(emitCompleteSignal()));
        }
    };
}

/* public slots */
QDomDocument HostDevice::getDataDocument() const
{
    QDomDocument doc;
    _QWidget *wdg = static_cast<_QWidget*>(info->currentWidget());
    if ( nullptr!=wdg ) doc = wdg->getDataDocument();
    return doc;
}

/* private slots */
void HostDevice::emitCompleteSignal()
{
    _QWidget *wdg = static_cast<_QWidget*>(sender());
    if ( wdg!=nullptr ) {
        ++completedWdg;
        if ( completedWdg>=info->count() ) {
            setEnabled(true);
            emit complete();
        }
    }
}

