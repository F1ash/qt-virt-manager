#include "char_device_edit.h"

CharDevice_Edit::CharDevice_Edit(
        QWidget *parent, virConnectPtr *connPtrPtr,
        virDomainPtr domain, QString _tag) :
    CharDevice(parent, connPtrPtr, domain, _tag)
{
    connect(devType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    for (int i=0; i<charDevWdg->count(); i++) {
        connect(charDevWdg->widget(i), SIGNAL(dataChanged()),
                this, SLOT(stateChanged()));
    };
}

/* public slots */
void CharDevice_Edit::setDataDescription(const QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device;
    _device = doc.firstChildElement("device")
            .firstChildElement(tag);
    QString _type = _device.attribute("type", "unix");
    int idx = devType->findData(
                _type,
                Qt::UserRole,
                Qt::MatchContains);
    devType->setCurrentIndex( (idx<0)? 0:idx );
    static_cast<_QWidget*>(charDevWdg->currentWidget())->setDataDescription(xmlDesc);
}
