#include "graphics_device_edit.h"

GraphicsDevice_Edit::GraphicsDevice_Edit(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    GraphicsDevice(parent, connPtrPtr)
{
    // dataChanged signals
    for (int i=0; i<info->count(); i++) {
        connect(info->widget(i), SIGNAL(dataChanged()),
                this, SLOT(stateChanged()));
    };
}

/* public slots */
void GraphicsDevice_Edit::setDataDescription(const QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _device;
    _device = doc.firstChildElement("device")
            .firstChildElement("graphics");
    QString _type = _device.attribute("type", "");
    int idx = type->findData(
                _type, Qt::UserRole, Qt::MatchExactly);
    type->setCurrentIndex( (idx<0)? 0:idx );
    type->setEnabled(false);
    _QWidget *wdg = static_cast<_QWidget*>(info->currentWidget());
    if ( Q_NULLPTR!=wdg ) wdg->setDataDescription(_xmlDesc);
}
