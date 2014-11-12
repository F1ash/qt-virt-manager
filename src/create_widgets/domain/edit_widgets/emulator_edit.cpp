#include "emulator_edit.h"

Emulator_Edit::Emulator_Edit(QWidget *parent) :
    Emulator(parent)
{
}

/* public slots */
void Emulator_Edit::setDataDescription(QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device;
    _device = doc.firstChildElement("device")
            .firstChildElement("emulator");
    emulator->setText(
                _device.firstChild().toText().data());
}
