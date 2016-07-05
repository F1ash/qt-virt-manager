#include "bch_host_device_edit.h"

BCh_Host_Device_Edit::BCh_Host_Device_Edit(QWidget *parent) :
    BCh_Host_Device(parent)
{
    connect(type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(device, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
}

/* public slots */
void BCh_Host_Device_Edit::setDataDescription(const QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _source, _el;
    _device = doc.firstChildElement("device")
            .firstChildElement("hostdev");
    QString _type = _device.attribute("type", "");
    int idx = type->findData(
                _type,
                Qt::UserRole,
                Qt::MatchContains);
    type->setCurrentIndex( (idx<0)? 0:idx );
    _source = _device.firstChildElement("source");
    QString _tag;
    switch (type->currentIndex()) {
    case 0:
        _tag = QString("block");
        break;
    case 1:
        _tag = QString("char");
        break;
    case 2:
        _tag = QString("interface");
        break;
    default:
        _tag = QString("block");
        break;
    };
    _el = _source.firstChildElement(_tag);
    device->setText(_el.firstChild().toText().data());
}
