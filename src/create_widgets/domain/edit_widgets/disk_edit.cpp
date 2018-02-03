#include "disk_edit.h"

Disk_Edit::Disk_Edit(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    Disk(parent, connPtrPtr)
{
    connect(source, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    for (int i=0; i<info->count(); i++) {
        connect(info->widget(i), SIGNAL(dataChanged()),
                this, SLOT(stateChanged()));
    };
}

/* public slots */
void Disk_Edit::setDataDescription(const QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _device;
    _device = doc.firstChildElement("device")
            .firstChildElement("disk");
    QString _source = _device.attribute("type");
    int idx = source->findData(
                _source, Qt::UserRole, Qt::MatchExactly);
    source->setCurrentIndex( (idx<0)? 0:idx );
    source->setEnabled(false);
    _QWidget *wdg = static_cast<_QWidget*>(info->currentWidget());
    if ( nullptr!=wdg ) wdg->setDataDescription(_xmlDesc);
}
