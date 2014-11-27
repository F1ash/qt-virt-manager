#include "cpu.h"

CPU::CPU(QWidget *parent, QString _caps, QString _xmlDesc) :
    _QWidget(parent), capabilities(_caps), xmlDesc(_xmlDesc)
{
    setObjectName("CPU");
    logicCPULabel = new LogicalHostCPU(this, capabilities);
    cpuAlloc = new CPU_Allocation(this, capabilities);
    cpuModel = new CPU_Model(this);
    cpuTopology = new CPU_Topology(this);
    scrolledLayout = new QVBoxLayout(this);
    scrolledLayout->addWidget(logicCPULabel);
    scrolledLayout->addWidget(cpuAlloc);
    scrolledLayout->addWidget(cpuModel);
    scrolledLayout->addWidget(cpuTopology);
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
    connect(cpuModel, SIGNAL(cpuUseElements(bool)),
            cpuTopology, SLOT(setUsage(bool)));
    connect(cpuTopology, SIGNAL(valueChanged(int)),
            cpuAlloc, SLOT(setMaxVCPU(int)));
    connect(cpuTopology, SIGNAL(useTopology(bool)),
            cpuAlloc->vcpu, SLOT(setDisabled(bool)));
    // dataChanged connections
    connect(cpuAlloc, SIGNAL(dataChanged()),
            this, SIGNAL(dataChanged()));
    connect(cpuModel, SIGNAL(dataChanged()),
            this, SIGNAL(dataChanged()));
    connect(cpuTopology, SIGNAL(dataChanged()),
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
    QDomElement _data, _vcpu, _cpu, _model,
            _topology, _feature;
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
    QString _attr;
    if ( cpuModel->isUsed() ) {
        _cpu = doc.createElement("cpu");
        if ( cpuModel->copyHostCPU->isChecked() ) {
            _cpu.setAttribute("mode", "host-passthrough");
        } else {
            if ( !cpuModel->getMode().isEmpty() )
                _cpu.setAttribute(
                            "mode", cpuModel->getMode());
            if ( !cpuModel->getMatch().isEmpty() )
                _cpu.setAttribute( "match", cpuModel->getMatch() );
            _model = doc.createElement("model");
            _attr = cpuModel->getModel();
            if ( !_attr.isEmpty() ) {
                data = doc.createTextNode(_attr);
                _model.appendChild(data);
            };
            _attr = ( cpuModel->allowFallback->isChecked() )?
                        "allow":"forbid";
            _model.setAttribute("fallback", _attr);
            _cpu.appendChild(_model);
            if ( cpuTopology->isUsed() ) {
                _topology = doc.createElement("topology");
                _topology.setAttribute(
                            "sockets",
                            cpuTopology->sockets->value());
                _topology.setAttribute(
                            "cores",
                            cpuTopology->cores->value());
                _topology.setAttribute(
                            "threads",
                            cpuTopology->threads->value());
                _cpu.appendChild(_topology);
            };
            //_feature = doc.createElement("feature");
            //_cpu.appendChild(_feature);
        };
    };
    if ( !_cpu.isNull() ) _data.appendChild(_cpu);
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
    cpuTopology->setMaxVCPU(_vcpu.toInt());
}
void CPU::changeArch(QString &_arch)
{
    cpuModel->archChanged(_arch);
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
    QDomElement _domain, _cpu, _vcpu, _model,
            _topology, _feature;
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
    cpuModel->setUsage(false);
    if ( !_cpu.isNull() ) {
        _attr = _cpu.attribute("mode");
        cpuModel->copyHostCPU->setChecked( _attr=="host-passthrough" );
        cpuModel->setUsage(true);
        if ( _attr!="host-passthrough" ) {
            _model = _cpu.firstChildElement("model");
            if ( !_model.isNull() ) {
                cpuModel->setUsage(true);
                _attr = _model.firstChild().toText().data();
                cpuModel->setModel( _attr );
                _attr = _model.attribute("fallback");
                cpuModel->allowFallback->setChecked( _attr=="allow" );
            };
            if ( _cpu.hasAttribute("match") ) {
                cpuModel->setUsage(true);
                _attr = _cpu.attribute("match");
                cpuModel->setMatch( _attr );
            };
            _feature = _cpu.firstChildElement("feature");
            if ( !_feature.isNull() ) {
                cpuModel->setUsage(true);
            };
            _topology = _cpu.firstChildElement("topology");
            if ( !_topology.isNull() ) {
                cpuModel->setUsage(true);
                cpuTopology->setUsage(true);
                cpuTopology->sockets->setValue(
                            _topology.attribute("sockets").toInt());
                cpuTopology->cores->setValue(
                            _topology.attribute("cores").toInt());
                cpuTopology->threads->setValue(
                            _topology.attribute("threads").toInt());
            };
        } else
            cpuModel->setUsage(true);
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
