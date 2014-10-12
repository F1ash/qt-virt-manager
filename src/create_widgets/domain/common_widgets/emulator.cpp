#include "emulator.h"

Emulator::Emulator(QWidget *parent, virConnectPtr conn) :
    _QWidget(parent, conn)
{
    hostArch = new QLabel(this);
    emulLabel = new QLabel("Available emulatores:", this);
    emulatores = new QComboBox(this);
    guestEmul = new QLabel("Guest Emulator: ---", this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(hostArch);
    commonLayout->addWidget(emulLabel, 0, Qt::AlignHCenter);
    commonLayout->addWidget(emulatores);
    commonLayout->addWidget(guestEmul);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(emulatores, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(guestEmulChanged(QString)));
    setAvailableEmulatores();
}

/* public slots */
QDomDocument Emulator::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _device, _emulator;
    _device = doc.createElement("device");
    _emulator = doc.createElement("emulator");
    QDomText data = doc.createTextNode(
                emulatores->currentText());
    _emulator.appendChild(data);
    _device.appendChild(_emulator);
    doc.appendChild(_device);
    return doc;
}

/* private slots */
void Emulator::setAvailableEmulatores()
{
    QString capabilities;
    capabilities = QString(virConnectGetCapabilities(currWorkConnect));
    //qDebug()<<capabilities;
    QDomDocument doc = QDomDocument();
    doc.setContent(capabilities);
    QString arch = doc.firstChildElement("capabilities").
               firstChildElement("host").
               firstChildElement("cpu").
               firstChildElement("arch").
               firstChild().toText().data();
    hostArch->setText(QString("Host architecture: %1").arg(arch));
    QDomElement el = doc.firstChildElement("capabilities").
               firstChildElement("guest");
    uint i = 0;
    int Idx = -1;
    while ( !el.isNull() ) {
        QDomElement _el = el.firstChildElement("arch");
        if ( !_el.isNull() ) {
            QString emulator = _el.
                    firstChildElement("emulator").
                    firstChild().toText().data();
            emulatores->addItem(emulator);
            /*  search emulator from guests description  */
            if ( arch==_el.attribute("name") ) {
                Idx = i;
            };
            i++;
        };
        el = el.nextSiblingElement("guest");
    };
    // set emulator for host arch
    if ( Idx>=0 ) emulatores->setCurrentIndex(Idx);
}
void Emulator::guestEmulChanged(QString _emul)
{
    guestEmul->setText(QString("Guest Emulator: %1").arg(_emul));
}
