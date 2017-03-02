#include "emulator_edit.h"

Emulator_Edit::Emulator_Edit(QWidget *parent) :
    Emulator(parent)
{
}

/* public slots */
void Emulator_Edit::setDataDescription(const QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _device;
    _device = doc.firstChildElement("device")
            .firstChildElement("emulator");
    emulator->setText(
                _device.firstChild().toText().data());
}
