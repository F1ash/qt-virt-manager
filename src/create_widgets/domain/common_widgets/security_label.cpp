#include "security_label.h"

SecurityLabel::SecurityLabel(
        QWidget *parent, QString _xmlDesc) :
    _Tab(parent), xmlDesc(_xmlDesc)
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
            this, SLOT(resetData()));
    connect(restorePanel, SIGNAL(revertData()),
            this, SLOT(revertData()));
    connect(restorePanel, SIGNAL(saveData()),
            this, SLOT(saveData()));
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
void SecurityLabel::setDataDescription(const QString &_xmlDesc)
{
    currentDeviceXMLDesc = _xmlDesc;
    readXMLDesciption(currentDeviceXMLDesc);
}

/* private slots */
void SecurityLabel::readXMLDesciption()
{
    currentDeviceXMLDesc = xmlDesc;
    readXMLDesciption(currentDeviceXMLDesc);
}
void SecurityLabel::readXMLDesciption(const QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    secLabels->readXMLDesciption(_xmlDesc);
}
