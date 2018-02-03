#include "console_device_edit.h"

ConsoleDevice_Edit::ConsoleDevice_Edit(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    ConsoleDevice(parent, connPtrPtr)
{
    connect(targetType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
}

/* public slots */
void ConsoleDevice_Edit::setDataDescription(const QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _device, _target;
    _device = doc.firstChildElement("device")
            .firstChildElement("console");
    _target = _device.firstChildElement("target");
    if ( !_target.isNull() ) {
        QString _console = _target.attribute("type", "");
        int idx = targetType->findData(
                _console, Qt::UserRole, Qt::MatchExactly);
        targetType->setCurrentIndex( (idx<0)? 0:idx );
    };
}
