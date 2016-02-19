#include "host_device_edit.h"

HostDevice_Edit::HostDevice_Edit(
        QWidget *parent, virConnectPtr *connPtrPtr) :
    HostDevice(parent, connPtrPtr)
{
    info->setVisible(false);
    infoEdit = new QStackedWidget(this);
    QString connType;
    if ( nullptr!=ptr_ConnPtr && nullptr!=*ptr_ConnPtr ) {
        connType = QString(virConnectGetType(*ptr_ConnPtr)).toLower();
    } else
        emit ptrIsNull();
    if ( connType=="qemu" ) {
        infoEdit->addWidget(new USB_Host_Device_Edit(this));
        infoEdit->addWidget(new PCI_Host_Device_Edit(this));
        infoEdit->addWidget(new SCSI_Host_Device_Edit(this, ptr_ConnPtr));
    } else if ( connType=="lxc" ) {
        infoEdit->addWidget(new USB_Host_Device_Edit(this));
        infoEdit->addWidget(new BCh_Host_Device_Edit(this));
    };
    commonLayout->insertWidget(2, infoEdit);
    connect(type, SIGNAL(currentIndexChanged(int)),
            infoEdit, SLOT(setCurrentIndex(int)));
    for (int i=0; i<infoEdit->count(); i++) {
        connect(infoEdit->widget(i), SIGNAL(dataChanged()),
                this, SLOT(stateChanged()));
    };
}

/* public slots */
QDomDocument HostDevice_Edit::getDataDocument() const
{
    QDomDocument doc;
    _QWidget *wdg = static_cast<_QWidget*>(infoEdit->currentWidget());
    if ( nullptr!=wdg ) doc = wdg->getDataDocument();
    return doc;
}
void HostDevice_Edit::setDataDescription(QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device;
    _device = doc.firstChildElement("device")
            .firstChildElement("hostdev");
    QString _type = _device.attribute("type", "");
    int idx = type->findText(_type, Qt::MatchContains);
    type->setCurrentIndex( (idx<0)? type->count()-1:idx );
    type->setEnabled(false);
    _QWidget *wdg = static_cast<_QWidget*>(infoEdit->currentWidget());
    if ( nullptr!=wdg ) wdg->setDataDescription(xmlDesc);
}
