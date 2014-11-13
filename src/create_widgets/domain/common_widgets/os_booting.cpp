#include "os_booting.h"

OS_Booting::OS_Booting(QWidget *parent, QString _caps, QString _xmlDesc) :
    _QWidget(parent), capabilities(_caps), xmlDesc(_xmlDesc)
{
    setObjectName("OS_Booting");
    readCapabilities();
    bootType = new QComboBox(this);
    bootType->addItem("BIOS bootloader", "bios");
    bootType->addItem("Host bootloader", "host");
    bootType->addItem("Direct kernel boot", "kernel");
    bootType->addItem("Container boot", "container");
    bootSet = new QStackedWidget(this);
    bootSet->addWidget(new BIOS_Boot(this, capabilities));
    bootSet->addWidget(new Host_Boot(this));
    bootSet->addWidget(new Direct_Kernel_Boot(this));
    bootSet->addWidget(new LXC_OSBooting(this, capabilities));
    bootSet->widget(0)->setEnabled(type.toLower()!="lxc");
    bootSet->widget(1)->setEnabled(type.toLower()!="lxc");
    bootSet->widget(2)->setEnabled(type.toLower()!="lxc");
    bootSet->widget(3)->setEnabled(type.toLower()=="lxc");
    connect(bootType, SIGNAL(currentIndexChanged(int)),
            bootSet, SLOT(setCurrentIndex(int)));
    connect(bootType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeBootType()));
    scrolledLayout = new QVBoxLayout(this);
    scrolledLayout->addWidget(bootType, 0, Qt::AlignLeft);
    scrolledLayout->addWidget(bootSet);
    scrolledLayout->addStretch(-1);
    scrolled = new QWidget(this);
    scrolled->setLayout(scrolledLayout);
    restorePanel = new RestorePanel(this);
    commonWdg = new QScrollArea(this);
    commonWdg->setWidget(scrolled);
    commonWdg->setWidgetResizable(true);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(restorePanel, 0, Qt::AlignRight);
    commonLayout->addWidget(commonWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    readXMLDesciption();
    // dataChanged connections
    connect(this, SIGNAL(dataChanged()),
            restorePanel, SLOT(stateChanged()));
    // action connections
    connect(restorePanel, SIGNAL(resetData()),
            this, SLOT(resetSecData()));
    connect(restorePanel, SIGNAL(revertData()),
            this, SLOT(revertSecData()));
    connect(restorePanel, SIGNAL(saveData()),
            this, SLOT(saveSecData()));
    for (uint i=0; i<bootSet->count(); i++) {
        connect(bootSet->widget(i), SIGNAL(domainType(QString&)),
                this, SIGNAL(domainType(QString&)));
        connect(bootSet->widget(i), SIGNAL(osType(QString&)),
                this, SLOT(changeOSType(QString&)));
        connect(bootSet->widget(i), SIGNAL(emulatorType(QString&)),
                this, SIGNAL(emulatorType(QString&)));
        connect(bootSet->widget(i), SIGNAL(dataChanged()),
                this, SLOT(stateChanged()));
    };
}

/* public slots */
QDomDocument OS_Booting::getDataDocument() const
{
    _QWidget *wdg = static_cast<_QWidget*>(
                bootSet->currentWidget());
    return wdg->getDataDocument();
}
QString OS_Booting::closeDataEdit()
{
    if ( !currentStateSaved ) {
        int answer = QMessageBox::question(
                    this,
                    "Save Memory Data",
                    "Save last changes?",
                    QMessageBox::Ok,
                    QMessageBox::Cancel);
        if ( answer==QMessageBox::Ok )
            saveSecData();
        else
            revertSecData();
    };
    return QString();
}

/* private slots */
void OS_Booting::readCapabilities()
{
    QDomDocument doc;
    doc.setContent(capabilities);
    QDomElement _domain = doc.
            firstChildElement("capabilities").
            firstChildElement("guest").
            firstChildElement("arch").
            firstChildElement("domain");
    if ( !_domain.isNull() ) {
        type = _domain.attribute("type");
    };
    arch = doc.firstChildElement("capabilities").
               firstChildElement("host").
               firstChildElement("cpu").
               firstChildElement("arch").
               firstChild().toText().data();
    os_type = doc.firstChildElement("capabilities").
               firstChildElement("guest").
               firstChildElement("os_type").
               firstChild().toText().data();
}
void OS_Booting::stateChanged()
{
    if ( currentStateSaved ) {
        currentStateSaved = false;
    };
    emit dataChanged();
}
void OS_Booting::readXMLDesciption()
{
    currentDeviceXMLDesc = xmlDesc;
    readXMLDesciption(currentDeviceXMLDesc);
}
void OS_Booting::readXMLDesciption(QString &xmlDesc)
{
    //if ( _xmlDesc.isEmpty() ) return;
    QDomDocument doc;
    QDomElement _domain, _os, _type;
    doc.setContent(xmlDesc);
    _domain = doc.firstChildElement("domain");
    _os = _domain.firstChildElement("os");
    _type = _os.firstChildElement("type");
    QString _bootType;
    if ( !_domain.firstChildElement("bootloader").isNull() ) {
        _bootType.append("host");
    } else if ( !_type.isNull() &&
                _type.firstChild().toText().data()=="exe" ) {
        _bootType.append("container");
    } else if ( !_type.isNull() &&
                !_os.firstChildElement("kernel")
                .isNull() ) {
        _bootType.append("kernel");
    };
    int idx = bootType->findData(
                _bootType,
                Qt::UserRole,
                Qt::MatchContains);
    bootType->setCurrentIndex( (idx<0)? 0:idx );
    static_cast<_QWidget*>(bootSet->currentWidget())->setDataDescription(xmlDesc);
}
void OS_Booting::resetSecData()
{
    readXMLDesciption();
    currentStateSaved = true;
    restorePanel->stateChanged(false);
}
void OS_Booting::revertSecData()
{
    readXMLDesciption(currentDeviceXMLDesc);
    currentStateSaved = true;
    restorePanel->stateChanged(false);
}
void OS_Booting::saveSecData()
{
    QDomDocument doc;
    QDomElement _os;
    doc = this->getDataDocument();
    _os = doc.firstChildElement("data");
    _os.setTagName("domain");
    currentDeviceXMLDesc = doc.toString();
    currentStateSaved = true;
    restorePanel->stateChanged(false);
}
void OS_Booting::changeOSType(QString &_type)
{
    os_type = _type;
    //qDebug()<<os_type;
}
void OS_Booting::changeBootType()
{
    QString _empty;
    emit emulatorType(_empty);
    emit domainType(_empty);
    static_cast<_QWidget*>(bootSet->currentWidget())->setInitState();
}
