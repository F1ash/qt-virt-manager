#include "misc_settings.h"

Misc_Settings::Misc_Settings(QWidget *parent, QString _caps, QString _xmlDesc) :
    _Tab(parent), capabilities(_caps), xmlDesc(_xmlDesc)
{
    setObjectName("Misc.");
    readCapabilities();
    eventsLabel = new QLabel(tr("Events configuration"), this);
    powerLabel = new QCheckBox(tr("Power Management"), this);
    powerLabel->setEnabled( type.compare("qemu")==0
                            || type.compare("kvm")==0 );
    featuresLabel = new QCheckBox(tr("Hypervisor features"), this);
    eventsWdg = new Events(this);
    powerWdg = new Power(this);
    powerWdg->setEnabled(false);
    featuresWdg = new HV_Features(this);
    featuresWdg->setEnabled(false);
    scrolledLayout = new QVBoxLayout(this);
    scrolledLayout->addWidget(eventsLabel);
    scrolledLayout->addWidget(eventsWdg);
    scrolledLayout->addWidget(powerLabel);
    scrolledLayout->addWidget(powerWdg);
    scrolledLayout->addWidget(featuresLabel);
    scrolledLayout->addWidget(featuresWdg);
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
    setLayout(commonLayout);
    connect(powerLabel, SIGNAL(toggled(bool)),
            powerWdg, SLOT(setEnabled(bool)));
    connect(featuresLabel, SIGNAL(toggled(bool)),
            featuresWdg, SLOT(setEnabled(bool)));
    readXMLDesciption();
    // action connections
    connect(restorePanel, SIGNAL(resetData()),
            this, SLOT(resetData()));
    connect(restorePanel, SIGNAL(revertData()),
            this, SLOT(revertData()));
    connect(restorePanel, SIGNAL(saveData()),
            this, SLOT(saveData()));
    // dataChanged connections
    connect(powerLabel, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(featuresLabel, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(eventsWdg, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(powerWdg, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(featuresWdg, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(this, SIGNAL(dataChanged()),
            restorePanel, SLOT(stateChanged()));
}

/* public slots */
QDomDocument Misc_Settings::getDataDocument() const
{
    QDomDocument doc;
    QDomText data;
    QDomElement _data, _on_poweroff, _on_reboot,
            _on_crash, _on_lockfailure, _pm, _features;
    _data = doc.createElement("data");
    if ( eventsWdg->powerOffLabel->isChecked() ) {
        _on_poweroff = doc.createElement("on_poweroff");
        data = doc.createTextNode(
                    eventsWdg->on_powerOff->currentText());
        _on_poweroff.appendChild(data);
        _data.appendChild(_on_poweroff);
    };
    if ( eventsWdg->rebootLabel->isChecked() ) {
        _on_reboot = doc.createElement("on_reboot");
        data = doc.createTextNode(
                    eventsWdg->on_reboot->currentText());
        _on_reboot.appendChild(data);
        _data.appendChild(_on_reboot);
    };
    if ( eventsWdg->crashLabel->isChecked() ) {
        _on_crash = doc.createElement("on_crash");
        data = doc.createTextNode(
                    eventsWdg->on_crash->currentText());
        _on_crash.appendChild(data);
        _data.appendChild(_on_crash);
    };
    if ( eventsWdg->lockFailureLabel->isChecked() ) {
        _on_lockfailure = doc.createElement("on_lockfailure");
        data = doc.createTextNode(
                    eventsWdg->on_lockfailure->currentText());
        _on_lockfailure.appendChild(data);
        _data.appendChild(_on_lockfailure);
    };
    if ( powerLabel->isChecked() ) {
        _pm = doc.createElement("pm");
        QDomElement _suspend_to_disk, _suspend_to_mem;
        _suspend_to_disk = doc.createElement("suspend-to-disk");
        _suspend_to_disk.setAttribute(
                    "enabled",
                    (powerWdg->suspend_to_disk->isChecked())? "yes":"no");
        _pm.appendChild(_suspend_to_disk);
        _suspend_to_mem = doc.createElement("suspend-to-mem");
        _suspend_to_mem.setAttribute(
                    "enabled",
                    (powerWdg->suspend_to_mem->isChecked())? "yes":"no");
        _pm.appendChild(_suspend_to_mem);
        _data.appendChild(_pm);
    };
    if ( featuresLabel->isChecked() ) {
        _features = doc.createElement("features");
        QDomDocument _doc;
        _doc.setContent(
                    featuresWdg->features->toPlainText());
        _features = _doc.firstChildElement("features");
        _data.appendChild(_features);
    };
    doc.appendChild(_data);
    //qDebug()<<doc.toString();
    return doc;
}

/* private slots */
void Misc_Settings::readCapabilities()
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
}
void Misc_Settings::readXMLDesciption()
{
    currentDeviceXMLDesc = xmlDesc;
    readXMLDesciption(currentDeviceXMLDesc);
}
void Misc_Settings::readXMLDesciption(const QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    QDomElement _domain, _on_poweroff, _on_reboot,
            _on_crash, _on_lockfailure, _pm, _features;
    doc.setContent(xmlDesc);
    _domain = doc.firstChildElement("domain");
    _on_poweroff = _domain.firstChildElement("on_poweroff");
    _on_reboot = _domain.firstChildElement("on_reboot");
    _on_crash = _domain.firstChildElement("on_crash");
    _on_lockfailure = _domain.firstChildElement("on_lockfailure");
    _pm = _domain.firstChildElement("pm");
    _features = _domain.firstChildElement("features");
    QString _attr;
    int idx;
    eventsWdg->powerOffLabel->setChecked( !_on_poweroff.isNull() );
    if ( !_on_poweroff.isNull() ) {
        _attr = _on_poweroff.firstChild().toText().data();
        idx = eventsWdg->on_powerOff->findText(
                    _attr, Qt::MatchContains);
        eventsWdg->on_powerOff->setCurrentIndex(
                    (idx<0)? 0:idx);
    };
    eventsWdg->rebootLabel->setChecked( !_on_reboot.isNull() );
    if ( !_on_reboot.isNull() ) {
        _attr = _on_reboot.firstChild().toText().data();
        idx = eventsWdg->on_reboot->findText(
                    _attr, Qt::MatchContains);
        eventsWdg->on_reboot->setCurrentIndex(
                    (idx<0)? 0:idx);
    };
    eventsWdg->crashLabel->setChecked( !_on_crash.isNull() );
    if ( !_on_crash.isNull() ) {
        _attr = _on_crash.firstChild().toText().data();
        idx = eventsWdg->on_crash->findText(
                    _attr, Qt::MatchContains);
        eventsWdg->on_crash->setCurrentIndex(
                    (idx<0)? 0:idx);
    };
    eventsWdg->lockFailureLabel->setChecked( !_on_lockfailure.isNull() );
    if ( !_on_lockfailure.isNull() ) {
        _attr = _on_lockfailure.firstChild().toText().data();
        idx = eventsWdg->on_lockfailure->findText(
                    _attr, Qt::MatchContains);
        eventsWdg->on_lockfailure->setCurrentIndex(
                    (idx<0)? 0:idx);
    };
    powerLabel->setChecked( !_pm.isNull() );
    if ( !_pm.isNull() ) {
        QDomElement _suspend_to_disk, _suspend_to_mem;
        _suspend_to_disk = _pm.firstChildElement("suspend-to-disk");
        _suspend_to_mem = _pm.firstChildElement("suspend-to-mem");
        powerWdg->suspend_to_disk->setChecked(
                    _suspend_to_disk.attribute("enabled").compare("yes")==0);
        powerWdg->suspend_to_mem->setChecked(
                    _suspend_to_mem.attribute("enabled").compare("yes")==0);
    };
    featuresLabel->setChecked( !_features.isNull() );
    if ( !_features.isNull() ) {
        QDomDocument _doc;
        _doc.setContent(QString());
        _doc.appendChild(_features);
        featuresWdg->setFeatures(_doc);
    };
}
