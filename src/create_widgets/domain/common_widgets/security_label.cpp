#include "security_label.h"

SecurityLabel::SecurityLabel(
        QWidget *parent, QString _xmlDesc) :
    _QWidget(parent), xmlDesc(_xmlDesc)
{
    setObjectName("Security-High");

    restorePanel = new RestorePanel(this);
    secLabels = new SecLabels(this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(restorePanel, 0, Qt::AlignRight);
    commonLayout->addWidget(secLabels);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    readXMLDesciption();
    // dataChanged connections
    connect(secLabels, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(this, SIGNAL(dataChanged()),
            restorePanel, SLOT(stateChanged()));
    // action connections
    connect(restorePanel, SIGNAL(resetData()),
            this, SLOT(resetSecData()));
    connect(restorePanel, SIGNAL(revertData()),
            this, SLOT(revertSecData()));
    connect(restorePanel, SIGNAL(saveData()),
            this, SLOT(saveSecData()));
}

/* public slots */
bool SecurityLabel::isUsed() const
{
    return secLabels->isUsed();
}
void SecurityLabel::setUsage(bool state)
{
    secLabels->setUsage(state);
}
QDomDocument SecurityLabel::getDataDocument() const
{
    return secLabels->getDataDocument();
}
void SecurityLabel::setDataDescription(QString &_xmlDesc)
{
    currentDeviceXMLDesc = _xmlDesc;
    readXMLDesciption(currentDeviceXMLDesc);
}
QString SecurityLabel::closeDataEdit()
{
    if ( !currentStateSaved ) {
        int answer = QMessageBox::question(
                    this,
                    "Save Security Label Data",
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
void SecurityLabel::stateChanged()
{
    if ( currentStateSaved ) {
        currentStateSaved = false;
    };
    emit dataChanged();
}
void SecurityLabel::readXMLDesciption()
{
    currentDeviceXMLDesc = xmlDesc;
    readXMLDesciption(currentDeviceXMLDesc);
}
void SecurityLabel::readXMLDesciption(QString &_xmlDesc)
{
    //if ( _xmlDesc.isEmpty() ) return;
    //qDebug()<<_xmlDesc;
    secLabels->readXMLDesciption(_xmlDesc);
}
void SecurityLabel::resetSecData()
{
    readXMLDesciption();
    currentStateSaved = true;
    restorePanel->stateChanged(false);
}
void SecurityLabel::revertSecData()
{
    readXMLDesciption(currentDeviceXMLDesc);
    currentStateSaved = true;
    restorePanel->stateChanged(false);
}
void SecurityLabel::saveSecData()
{
    QDomDocument doc;
    QDomElement _domain;
    doc = this->getDataDocument();
    _domain = doc.firstChildElement("data");
    _domain.setTagName("domain");
    currentDeviceXMLDesc = doc.toString();
    currentStateSaved = true;
    restorePanel->stateChanged(false);
}
