#include "emulator.h"

Emulator::Emulator(QWidget *parent) :
    _QWidget(parent)
{
    emulLabel = new QLabel(tr("Emulator:"), this);
    emulator = new QLabel(this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(emulLabel, 0, Qt::AlignHCenter);
    commonLayout->addWidget(emulator);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
}

/* public slots */
QDomDocument Emulator::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _device, _emulator;
    _device = doc.createElement("device");
    _emulator = doc.createElement("emulator");
    QDomText data = doc.createTextNode(
                emulator->text());
    _emulator.appendChild(data);
    _device.appendChild(_emulator);
    doc.appendChild(_device);
    return doc;
}
