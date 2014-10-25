#include "cpu.h"

CPU::CPU(QWidget *parent, QString _xmlDesc) :
    _QWidget(parent), xmlDesc(_xmlDesc)
{
    setObjectName("CPU");
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
QDomDocument CPU::getDataDocument() const
{
    QDomDocument doc, _cpuDesc;
    QDomElement _data;
    _data = doc.createElement("data");
    _cpuDesc.setContent(editor->toPlainText());
    _data.appendChild(_cpuDesc);
    doc.appendChild(_data);
    //qDebug()<<doc.toString();
    return doc;
}
QString CPU::closeDataEdit()
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
void CPU::stateChanged()
{
    if ( currentStateSaved ) {
        currentStateSaved = false;
    };
    emit dataChanged();
}
void CPU::readXMLDesciption()
{
    currentDeviceXMLDesc = xmlDesc;
    readXMLDesciption(currentDeviceXMLDesc);
}
void CPU::readXMLDesciption(QString &_xmlDesc)
{
    //if ( _xmlDesc.isEmpty() ) return;
    QDomDocument doc, _cpuDesc;
    QDomElement _domain, _cpu;
    doc.setContent(_xmlDesc);
    _domain = doc.firstChildElement("domain");
    _cpu = _domain.firstChildElement("cpu");
    _cpuDesc.setContent(QString(""));
    _cpuDesc.appendChild(_cpu);
    QString _description = QString(
                _cpuDesc.toDocument().toByteArray(4).data());
    editor->setText(_description);
}
void CPU::resetSecData()
{
    readXMLDesciption();
    currentStateSaved = true;
    restorePanel->stateChanged(false);
}
void CPU::revertSecData()
{
    readXMLDesciption(currentDeviceXMLDesc);
    currentStateSaved = true;
    restorePanel->stateChanged(false);
}
void CPU::saveSecData()
{
    QDomDocument doc;
    QDomElement _cpu;
    doc = this->getDataDocument();
    _cpu = doc.firstChildElement("data");
    _cpu.setTagName("domain");
    currentDeviceXMLDesc = doc.toString();
    currentStateSaved = true;
    restorePanel->stateChanged(false);
}

