#include "_arch.h"

_Arch::_Arch(QWidget *parent, QString _caps) :
    _QWidget(parent), capabilities(_caps)
{
    doc.setContent(capabilities);
    archLabel = new QLabel("Architectures:", this);
    machLabel = new QLabel("Machines:", this);
    arch = new QComboBox(this);
    machines = new QComboBox(this);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(archLabel, 0, 0);
    commonLayout->addWidget(machLabel, 0, 1);
    commonLayout->addWidget(arch, 1, 0);
    commonLayout->addWidget(machines, 1, 1);
    setLayout(commonLayout);
    connect(arch, SIGNAL(currentIndexChanged(const QString&)),
            this, SLOT(archChanged(const QString&)));
    connect(machines, SIGNAL(currentIndexChanged(const QString&)),
            this, SLOT(machineChanged(const QString&)));
    // dataChanged connections
    connect(arch, SIGNAL(currentIndexChanged(const QString&)),
            this, SLOT(stateChanged()));
    connect(machines, SIGNAL(currentIndexChanged(const QString&)),
            this, SLOT(stateChanged()));
}

/* public slots */
void _Arch::setItems()
{
    arch->clear();
    QDomElement _el = doc
            .firstChildElement("capabilities")
            .firstChildElement("guest");
    while ( !_el.isNull() ) {
        arch->addItem(
                    _el
                    .firstChildElement("arch")
                    .attribute("name"));
        _el = _el.nextSiblingElement("guest");
    };
    machineChanged(_el
            .firstChildElement("arch")
            .firstChildElement("domain")
            .firstChildElement("machine")
            .firstChild().toText().data());
}
void _Arch::setArch(const QString &_arch)
{
    int idx = arch->findText(_arch, Qt::MatchContains);
    arch->setCurrentIndex( (idx<0)? 0:idx );
}
void _Arch::setMachine(const QString &_mach)
{
    int idx = machines->findText(_mach, Qt::MatchContains);
    machines->setCurrentIndex( (idx<0)? 0:idx );
}
QString _Arch::getMachine() const
{
    return machines->currentText();
}

/* private slots */
void _Arch::archChanged(const QString &_arch)
{
    QString _domType, _osType, _emulType, _Arch;
    _Arch = _arch;
    machines->clear();
    QDomElement _el = doc
            .firstChildElement("capabilities")
            .firstChildElement("guest");
    while ( !_el.isNull() ) {
        _osType = _el
                .firstChildElement("os_type")
                .firstChild().toText().data();
        if ( _el
             .firstChildElement("arch")
             .attribute("name")==_arch ) {
            QDomElement _domain, _ell;
            _domain = _el
                    .firstChildElement("arch")
                    .firstChildElement("domain");
            bool exist = false;
            while ( !_domain.isNull() ) {
                _domType = _domain.attribute("type");
                _emulType = _domain
                        .firstChildElement("emulator")
                        .firstChild().toText().data();
                // WARNING: in this application
                // KVM has the advantage over QEMU
                if ( _domType=="kvm" ) {
                    _ell = _domain
                            .firstChildElement("machine");
                    exist = true;
                    break;
                };
                _domain = _domain.nextSiblingElement("domain");
            };
            if ( !exist ) {
                _emulType = _el
                        .firstChildElement("arch")
                        .firstChildElement("emulator")
                        .firstChild().toText().data();
                _ell = _el
                        .firstChildElement("arch")
                        .firstChildElement("machine");
            };
            while ( !_ell.isNull() ) {
                machines->addItem(
                    _ell
                    .firstChild().toText().data());
                _ell = _ell.nextSiblingElement("machine");
            };
            break;
        };
        _el = _el.nextSiblingElement("guest");
    };
    emit domainType(_domType);
    emit archType(_Arch);
    emit osType(_osType);
    emit emulatorType(_emulType);
}
void _Arch::machineChanged(const QString &_machine)
{
    QString _arch, _vcpu;
    _arch = arch->currentText();
    QDomElement _el = doc
            .firstChildElement("capabilities")
            .firstChildElement("guest");
    while ( !_el.isNull() ) {
        if ( _el
             .firstChildElement("arch")
             .attribute("name")==_arch ) {
            QDomElement _domain, _ell;
            _domain = _el
                    .firstChildElement("arch")
                    .firstChildElement("domain");
            bool exist = false;
            while ( !_domain.isNull() ) {
                QString _domType = _domain.attribute("type");
                // WARNING: in this application
                // KVM has the advantage over QEMU
                if ( _domType=="kvm" ) {
                    _ell = _domain
                            .firstChildElement("machine");
                    exist = true;
                    break;
                };
                _domain = _domain.nextSiblingElement("domain");
            };
            if ( !exist ) {
                _ell = _el
                        .firstChildElement("arch")
                        .firstChildElement("machine");
            };
            while ( !_ell.isNull() ) {
                if ( machines->currentText()==_machine ) {
                    _vcpu = _ell.attribute("maxCpus");
                    break;
                };
                _ell = _ell.nextSiblingElement("machine");
            };
            break;
        };
        _el = _el.nextSiblingElement("guest");
    };
    //qDebug()<<_vcpu;
    emit maxVCPU(_vcpu);
}
