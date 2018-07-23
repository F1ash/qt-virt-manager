#include "os_booting.h"

OS_Booting::OS_Booting(
        QWidget *parent,
        QString _caps,
        QString _xmlDesc) :
    _Tab(parent), capabilities(_caps), xmlDesc(_xmlDesc)
{
    setObjectName("OS_Booting");
    readCapabilities();
    bootType = new BootType(this);
    bootSet = new QStackedWidget(this);
    bootSet->addWidget(new BIOS_Boot(this, capabilities));
    bootSet->addWidget(new Host_Boot(this));
    bootSet->addWidget(new Direct_Kernel_Boot(this));
    bootSet->addWidget(new LXC_OSBooting(this, capabilities));
    bootSet->widget(0)->setEnabled(type.toLower().compare("lxc")!=0);
    bootSet->widget(1)->setEnabled(type.toLower().compare("lxc")!=0);
    bootSet->widget(2)->setEnabled(type.toLower().compare("lxc")!=0);
    bootSet->widget(3)->setEnabled(type.toLower().compare("lxc")==0);
    bootType->osType->setEnabled(type.toLower().compare("lxc")!=0);
    connect(bootType->bootType, SIGNAL(currentIndexChanged(int)),
            bootSet, SLOT(setCurrentIndex(int)));
    connect(bootType->bootType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeBootType()));
    //scrolledLayout = new QVBoxLayout(this);
    //scrolledLayout->addWidget(bootType);
    //scrolledLayout->addWidget(bootSet);
    //scrolledLayout->addStretch(-1);
    //scrolled = new QWidget(this);
    //scrolled->setLayout(scrolledLayout);
    restorePanel = new RestorePanel(this);
    //commonWdg = new QScrollArea(this);
    //commonWdg->setWidget(scrolled);
    //commonWdg->setWidgetResizable(true);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(restorePanel, 0, Qt::AlignRight);
    //commonLayout->addWidget(commonWdg);
    commonLayout->addWidget(bootType);
    commonLayout->addWidget(bootSet);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    readXMLDesciption();
    // dataChanged connections
    connect(this, SIGNAL(dataChanged()),
            restorePanel, SLOT(stateChanged()));
    connect(bootType->osType, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    // action connections
    connect(restorePanel, SIGNAL(resetData()),
            this, SLOT(resetData()));
    connect(restorePanel, SIGNAL(revertData()),
            this, SLOT(revertData()));
    connect(restorePanel, SIGNAL(saveData()),
            this, SLOT(saveData()));
    for (int i=0; i<bootSet->count(); i++) {
        connect(bootSet->widget(i), SIGNAL(domainType(const QString&)),
                this, SIGNAL(domainType(const QString&)));
        connect(bootSet->widget(i), SIGNAL(osType(const QString&)),
                this, SLOT(changeOSType(const QString&)));
        connect(bootSet->widget(i), SIGNAL(emulatorType(const QString&)),
                this, SIGNAL(emulatorType(const QString&)));
        connect(bootSet->widget(i), SIGNAL(dataChanged()),
                this, SLOT(stateChanged()));
    };
    BIOS_Boot *wdg = static_cast<BIOS_Boot*>(bootSet->widget(0));
    connect(wdg->architecture, SIGNAL(maxVCPU(const QString&)),
            this, SIGNAL(maxVCPU(const QString&)));
    connect(wdg->architecture, SIGNAL(archType(const QString&)),
            this, SIGNAL(archChanged(const QString&)));
}

/* public slots */
QDomDocument OS_Booting::getDataDocument() const
{
    QDomDocument doc;
    _QWidget *wdg = static_cast<_QWidget*>(bootSet->currentWidget());
    if ( Q_NULLPTR!=wdg ) doc = wdg->getDataDocument();
    QDomElement _os, _type;
    _os = doc
            .firstChildElement("data")
            .firstChildElement("os");
    QString _bootType = bootType->bootType->itemData(
                bootType->bootType->currentIndex(), Qt::UserRole)
            .toString();
    if ( _bootType.compare("host")!=0 ) {
        if ( !_os.isNull() ) {
            _type = _os
                    .firstChildElement("type");
            if ( !bootType->osType->text().isEmpty() ) {
                if ( _type.isNull() ) {
                    _type = doc.createElement("type");
                    _os.appendChild(_type);
                };
                QDomText _text = doc.createTextNode(
                            bootType->osType->text());
                _type.appendChild(_text);
            };
        };
    };
    //qDebug()<<doc.toString();
    return doc;
}
void OS_Booting::searchBootableDevices(QDomDocument &_doc)
{
    BIOS_Boot *wdg = static_cast<BIOS_Boot*>(bootSet->widget(0));
    if ( Q_NULLPTR!=wdg ) wdg->searchBootableDevices(_doc);
}
BootOrderList OS_Booting::getBootOrder() const
{
    BootOrderList _ret;
    int idx = bootSet->currentIndex();
    if ( idx==0 ) {
        BIOS_Boot *wdg = static_cast<BIOS_Boot*>(
                    bootSet->currentWidget());
        if ( Q_NULLPTR!=wdg ) {
            _ret = wdg->getBootOrderData();
        };
    };
    return _ret;
}
void OS_Booting::initMaxVCPU()
{
    if ( type.toLower().compare("lxc")==0 ) {
        LXC_OSBooting *wdg = static_cast<LXC_OSBooting*>(bootSet->widget(3));
        wdg->architecture->machineChanged(
                    wdg->architecture->getMachine());
        wdg->architecture->archChanged(
                    wdg->architecture->getArch());
    } else {
        BIOS_Boot *wdg = static_cast<BIOS_Boot*>(bootSet->widget(0));
        wdg->architecture->machineChanged(
                    wdg->architecture->getMachine());
        wdg->architecture->archChanged(
                    wdg->architecture->getArch());
    };
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
void OS_Booting::readXMLDesciption()
{
    currentDeviceXMLDesc = xmlDesc;
    readXMLDesciption(currentDeviceXMLDesc);
}
void OS_Booting::readXMLDesciption(const QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    QDomElement _domain, _os, _type;
    doc.setContent(_xmlDesc);
    _domain = doc.firstChildElement("domain");
    _os = _domain.firstChildElement("os");
    _type = _os.firstChildElement("type");
    QString _bootType;
    if ( !_domain.firstChildElement("bootloader").isNull() ) {
        _bootType.append("host");
    } else if ( !_type.isNull() &&
                _type.firstChild().toText().data().compare("exe")==0 ) {
        _bootType.append("container");
    } else if ( !_type.isNull() &&
                !_os.firstChildElement("kernel")
                .isNull() ) {
        _bootType.append("kernel");
    };
    if ( !_type.isNull() ) {
        QString _t = _type.firstChild().toText().data();
        changeOSType(_t);
    };
    int idx = bootType->bootType->findData(
                _bootType,
                Qt::UserRole,
                Qt::MatchContains);
    bootType->bootType->setCurrentIndex( (idx<0)? 0:idx );
    _QWidget *wdg = static_cast<_QWidget*>(bootSet->currentWidget());
    if ( wdg!=Q_NULLPTR ) wdg->setDataDescription(_xmlDesc);
}
void OS_Booting::changeOSType(const QString &_type)
{
    os_type = _type;
    //qDebug()<<os_type;
    bootType->osType->setText(os_type);
}
void OS_Booting::changeBootType()
{
    QString _empty, _type;
    emit emulatorType(_empty);
    emit domainType(_empty);
    _QWidget *wdg = static_cast<_QWidget*>(bootSet->currentWidget());
    if ( wdg!=Q_NULLPTR ) wdg->setInitState();
    _type = bootType->bootType->itemData(
                bootType->bootType->currentIndex(), Qt::UserRole)
            .toString();
    bootType->osType->setEnabled(
                _type.compare("host")!=0
                 && bootSet->currentWidget()->isEnabled());
}
