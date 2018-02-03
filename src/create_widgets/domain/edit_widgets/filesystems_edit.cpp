#include "filesystems_edit.h"

FileSystems_Edit::FileSystems_Edit(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    FileSystems(parent, connPtrPtr)
{
    connect(type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    for (int i=0; i<info->count(); i++) {
        connect(info->widget(i), SIGNAL(dataChanged()),
                this, SLOT(stateChanged()));
    };
}

/* public slots */
void FileSystems_Edit::setDataDescription(const QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _device;
    _device = doc.firstChildElement("device")
            .firstChildElement("filesystem");
    QString _type = _device.attribute("type");
    int idx = type->findText(_type, Qt::MatchContains);
    type->setCurrentIndex( (idx<0)? 0:idx );
    type->setEnabled(false);
    _QWidget *wdg = static_cast<_QWidget*>(info->currentWidget());
    if ( nullptr!=wdg ) wdg->setDataDescription(_xmlDesc);
}
