#include "cpu.h"

CPU::CPU(QWidget *parent, QString _caps, QString _xmlDesc) :
    _QWidget(parent), capabilities(_caps), xmlDesc(_xmlDesc)
{
    setObjectName("CPU");
    logicCPULabel = new LogicalHostCPU(this, capabilities);
    cpuAlloc = new CPU_Allocation(this, capabilities);
    scrolledLayout = new QVBoxLayout(this);
    scrolledLayout->addWidget(logicCPULabel);
    scrolledLayout->addWidget(cpuAlloc);
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
    readXMLDesciption();
    connect(cpuAlloc, SIGNAL(currentVCPU(int)),
            logicCPULabel, SLOT(changeInfoVisibility(int)));
    // dataChanged connections
    connect(cpuAlloc, SIGNAL(dataChanged()),
            this, SIGNAL(dataChanged()));
    connect(this, SIGNAL(dataChanged()),
            restorePanel, SLOT(stateChanged()));
    // action connections
    connect(restorePanel, SIGNAL(resetData()),
            this, SLOT(resetCPUData()));
    connect(restorePanel, SIGNAL(revertData()),
            this, SLOT(revertCPUData()));
    connect(restorePanel, SIGNAL(saveData()),
            this, SLOT(saveCPUData()));
}

/* public slots */
QDomDocument CPU::getDataDocument() const
{
    QDomDocument doc;
    QDomText data;
    QDomElement _data, _vcpu, _cpu;
    _data = doc.createElement("data");
    _vcpu = doc.createElement("vcpu");
    data = doc.createTextNode(cpuAlloc->vcpu->text());
    _vcpu.appendChild(data);
    if ( cpuAlloc->placementLabel->isChecked() ) {
        _vcpu.setAttribute(
                    "placement",
                    cpuAlloc->placement->currentText());
    };
    if ( cpuAlloc->cpusetLabel->isChecked() ) {
        _vcpu.setAttribute(
                    "cpuset",
                    cpuAlloc->cpuset->text());
    };
    if ( cpuAlloc->currLabel->isChecked() ) {
        _vcpu.setAttribute(
                    "current",
                    cpuAlloc->current->text());
    };
    _data.appendChild(_vcpu);
    doc.appendChild(_data);
    //qDebug()<<doc.toString();
    return doc;
}
QString CPU::closeDataEdit()
{
    if ( !currentStateSaved ) {
        int answer = QMessageBox::question(
                    this,
                    "Save CPU Data",
                    "Save last changes?",
                    QMessageBox::Ok,
                    QMessageBox::Cancel);
        if ( answer==QMessageBox::Ok )
            saveCPUData();
        else
            revertCPUData();
    };
    return QString();
}
void CPU::setMaxVCPU(QString &_vcpu)
{
    //qDebug()<<_vcpu;
    cpuAlloc->vcpu->setRange(1, _vcpu.toInt());
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
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    QDomElement _domain, _cpu, _vcpu;
    doc.setContent(_xmlDesc);
    _domain = doc.firstChildElement("domain");
    _vcpu = _domain.firstChildElement("vcpu");
    _cpu = _domain.firstChildElement("cpu");
    QString _attr;
    if ( !_vcpu.isNull() ) {
        _attr = _vcpu.firstChild().toText().data();
        cpuAlloc->vcpu->setValue(_attr.toInt());
        _attr = _vcpu.attribute("placement");
        cpuAlloc->placementLabel->setChecked( !_attr.isNull() );
        if ( !_attr.isNull() )
            cpuAlloc->setPlacement(_attr);
        _attr = _vcpu.attribute("cpuset");
        cpuAlloc->cpusetLabel->setChecked( !_attr.isNull() );
        if ( !_attr.isNull() )
            cpuAlloc->cpuset->setText(_attr);
        else
            cpuAlloc->cpuset->clear();
        _attr = _vcpu.attribute("current");
        cpuAlloc->currLabel->setChecked( !_attr.isNull() );
        if ( !_attr.isNull() )
            cpuAlloc->current->setValue(_attr.toInt());
    };
    if ( !_cpu.isNull() ) {

    };
}
void CPU::resetCPUData()
{
    readXMLDesciption();
    currentStateSaved = true;
    restorePanel->stateChanged(false);
}
void CPU::revertCPUData()
{
    readXMLDesciption(currentDeviceXMLDesc);
    currentStateSaved = true;
    restorePanel->stateChanged(false);
}
void CPU::saveCPUData()
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
