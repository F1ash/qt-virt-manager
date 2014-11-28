#include "misc_settings.h"

Misc_Settings::Misc_Settings(QWidget *parent, QString _caps, QString _xmlDesc) :
    _QWidget(parent), capabilities(_caps), xmlDesc(_xmlDesc)
{
    setObjectName("Misc.");
    eventsWdg = new Events(this);
    powerWdg = new Power(this);
    featuresWdg = new HV_Features(this);
    scrolledLayout = new QVBoxLayout(this);
    scrolledLayout->addWidget(eventsWdg);
    scrolledLayout->addWidget(powerWdg);
    scrolledLayout->addWidget(featuresWdg);
    //scrolledLayout->addStretch(-1);
    scrolled = new QWidget(this);
    scrolled->setLayout(scrolledLayout);
    restorePanel = new RestorePanel(this);
    commonWdg = new QScrollArea(this);
    commonWdg->setWidget(scrolled);
    commonWdg->setWidgetResizable(true);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(restorePanel, 0, Qt::AlignRight);
    commonLayout->addWidget(commonWdg);
    //commonLayout->addStretch(-1);
    setLayout(commonLayout);
    readXMLDesciption();
    // action connections
    connect(restorePanel, SIGNAL(resetData()),
            this, SLOT(resetMiscData()));
    connect(restorePanel, SIGNAL(revertData()),
            this, SLOT(revertMiscData()));
    connect(restorePanel, SIGNAL(saveData()),
            this, SLOT(saveMiscData()));
}

/* public slots */
QDomDocument Misc_Settings::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _os, _type;
    _os = doc
            .firstChildElement("data")
            .firstChildElement("os");
    //qDebug()<<doc.toString();
    return doc;
}
QString Misc_Settings::closeDataEdit()
{
    if ( !currentStateSaved ) {
        int answer = QMessageBox::question(
                    this,
                    "Save Misc. Data",
                    "Save last changes?",
                    QMessageBox::Ok,
                    QMessageBox::Cancel);
        if ( answer==QMessageBox::Ok )
            saveMiscData();
        else
            revertMiscData();
    };
    return QString();
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
}
void Misc_Settings::stateChanged()
{
    if ( currentStateSaved ) {
        currentStateSaved = false;
    };
    emit dataChanged();
}
void Misc_Settings::readXMLDesciption()
{
    currentDeviceXMLDesc = xmlDesc;
    readXMLDesciption(currentDeviceXMLDesc);
}
void Misc_Settings::readXMLDesciption(QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    QDomElement _domain, _os, _type;
    doc.setContent(xmlDesc);
    _domain = doc.firstChildElement("domain");
    _os = _domain.firstChildElement("os");
}
void Misc_Settings::resetMiscData()
{
    readXMLDesciption();
    currentStateSaved = true;
    restorePanel->stateChanged(false);
}
void Misc_Settings::revertMiscData()
{
    readXMLDesciption(currentDeviceXMLDesc);
    currentStateSaved = true;
    restorePanel->stateChanged(false);
}
void Misc_Settings::saveMiscData()
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
