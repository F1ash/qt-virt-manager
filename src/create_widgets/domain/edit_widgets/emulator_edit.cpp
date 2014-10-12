#include "emulator_edit.h"

Emulator_Edit::Emulator_Edit(QWidget *parent, virConnectPtr conn) :
    Emulator(parent, conn)
{
    connect(emulatores, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
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
    QString _emul = _device.firstChild()
            .toText().data();
    int idx = emulatores->findText(
                _emul, Qt::MatchExactly);
    emulatores->setCurrentIndex( (idx<0)? 0:idx );
}
