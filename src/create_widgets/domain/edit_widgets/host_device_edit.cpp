#include "host_device_edit.h"

HostDevice_Edit::HostDevice_Edit(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    HostDevice(parent, connPtrPtr)
{
    info->setVisible(false);
    info->blockSignals(true);
    infoEdit = new QStackedWidget(this);
    commonLayout->insertWidget(2, infoEdit);
}

/* public slots */
QDomDocument HostDevice_Edit::getDataDocument() const
{
    QDomDocument doc;
    _QWidget *wdg = static_cast<_QWidget*>(infoEdit->currentWidget());
    if ( nullptr!=wdg ) doc = wdg->getDataDocument();
    return doc;
}
void HostDevice_Edit::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _device;
    _device = doc.firstChildElement("device")
            .firstChildElement("hostdev");
    QString _type = _device.attribute("type", "");
    int idx = type->findText(_type, Qt::MatchContains);
    type->setCurrentIndex( (idx<0)? type->count()-1:idx );
    type->setEnabled(false);
    _QWidget *wdg = static_cast<_QWidget*>(infoEdit->currentWidget());
    if ( nullptr!=wdg ) wdg->setDataDescription(_xmlDesc);
}

/* private slots */
void HostDevice_Edit::init_wdg()
{
    if ( hlpThread->connType.toLower()=="qemu" ) {
        type->addItems(QEMU_DEV_LIST);
        infoEdit->addWidget(new USB_Host_Device_Edit(this));
        infoEdit->addWidget(new PCI_Host_Device_Edit(this));
        infoEdit->addWidget(new SCSI_Host_Device_Edit(this, ptr_ConnPtr));
    } else if ( hlpThread->connType.toLower()=="lxc" ) {
        type->addItems(LXC_DEV_LIST);
        infoEdit->addWidget(new USB_Host_Device_Edit(this));
        infoEdit->addWidget(new BCh_Host_Device_Edit(this));
    };
    connect(type, SIGNAL(currentIndexChanged(int)),
            infoEdit, SLOT(setCurrentIndex(int)));
    for (int i=0; i<infoEdit->count(); i++) {
        _QWidget *wdg = static_cast<_QWidget*>(infoEdit->widget(i));
        if ( wdg!=nullptr ) {
            connect(wdg, SIGNAL(dataChanged()),
                    this, SLOT(stateChanged()));
        };
    };
}
