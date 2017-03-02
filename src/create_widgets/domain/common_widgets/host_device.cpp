#include "host_device.h"

/*
 * http://libvirt.org/formatdomain.html#elementsHostDev
 */

HostDevice::HostDevice(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    _QWidget_Threaded(parent, connPtrPtr)
{
    type = new QComboBox(this);
    info = new QStackedWidget(this);
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
    hlpThread->start();
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
void HostDevice::init_wdg()
{
    if ( hlpThread->connType.toLower()=="qemu" ) {
        type->addItems(QEMU_DEV_LIST);
        info->addWidget(new USB_Host_Device(this, ptr_ConnPtr));
        info->addWidget(new PCI_Host_Device(this, ptr_ConnPtr));
        info->addWidget(new SCSI_Host_Device(this, ptr_ConnPtr));
    } else if ( hlpThread->connType.toLower()=="lxc" ) {
        type->addItems(LXC_DEV_LIST);
        info->addWidget(new USB_Host_Device(this, ptr_ConnPtr));
        info->addWidget(new BCh_Host_Device(this, ptr_ConnPtr));
    };
    connect(type, SIGNAL(currentIndexChanged(int)),
            info, SLOT(setCurrentIndex(int)));
    for(uint i=0; i<info->count(); ++i) {
        _QWidget *wdg = static_cast<_QWidget*>(info->widget(i));
        if ( wdg!=nullptr ) {
            connect(wdg, SIGNAL(complete()),
                    this, SLOT(emitCompleteSignal()));
        }
    };
}
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
