#include "os_booting.h"

OS_Booting::OS_Booting(
        QWidget *parent, QString _xmlDesc) :
    _QWidget(parent), xmlDesc(_xmlDesc)
{
    setObjectName("OS_Booting");
    // workaround
    editor = new QTextEdit(this);
    //
    scrolledLayout = new QVBoxLayout(this);
    scrolledLayout->addWidget(editor);
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
    connect(editor, SIGNAL(textChanged()),
            this, SIGNAL(dataChanged()));
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
QDomDocument OS_Booting::getDataDocument() const
{
    QDomDocument doc, _osDesc;
    QDomElement _data;
    _data = doc.createElement("data");
    _osDesc.setContent(editor->toPlainText());
    _data.appendChild(_osDesc);
    doc.appendChild(_data);
    //qDebug()<<doc.toString();
    return doc;
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
void OS_Booting::readXMLDesciption(QString &_xmlDesc)
{
    //if ( _xmlDesc.isEmpty() ) return;
    QDomDocument doc, _osDesc;
    QDomElement _domain, _os;
    doc.setContent(_xmlDesc);
    _domain = doc.firstChildElement("domain");
    _os = _domain.firstChildElement("os");
    _osDesc.setContent(QString(""));
    _osDesc.appendChild(_os);
    QString _description = QString(
                _osDesc.toDocument().toByteArray(4).data());
    editor->setText(_description);
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
