#include "_arch.h"
//#include <QTextStream>

_Arch::_Arch(QWidget *parent, QString _caps) :
    _QWidget(parent), capabilities(_caps)
{
    doc.setContent(capabilities);
    archLabel = new QLabel(tr("Architectures:"), this);
    machLabel = new QLabel(tr("Machines:"), this);
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
    //qDebug()<<_arch;
    int idx = arch->findText(_arch, Qt::MatchContains);
    arch->setCurrentIndex( (idx<0)? 0:idx );
}
void _Arch::setMachine(const QString &_mach)
{
    //qDebug()<<_mach;
    int idx = machines->findText(_mach, Qt::MatchContains);
    machines->setCurrentIndex( (idx<0)? 0:idx );
}
QString _Arch::getArch() const
{
    return arch->currentText();
}
QString _Arch::getMachine() const
{
    return machines->currentText();
}
void _Arch::machineChanged(const QString &_machine)
{
    //QTextStream s(stdout);
    //s<<"Machine :"<< _machine<<endl;
    QString _arch, _vcpu;
    _arch = arch->currentText();
    QDomElement _guest = doc
            .firstChildElement("capabilities")
            .firstChildElement("guest");
    while ( !_guest.isNull() ) {
        if ( _guest
             .firstChildElement("arch")
             .attribute("name").compare(_arch)==0 ) {
            QDomElement _domain, _ell;
            _domain = _guest
                    .firstChildElement("arch")
                    .firstChildElement("domain");
            bool exist = false;
            /*
            while ( !_domain.isNull() ) {
                QString _domType = _domain.attribute("type");
                // WARNING: in this application
                // KVM has the advantage over QEMU
                if ( _domType.compare("kvm")==0 ) {
                    _ell = _domain
                            .firstChildElement("machine");
                    exist = true;
                    break;
                };
                _domain = _domain.nextSiblingElement("domain");
            };
            */
            if ( !exist ) {
                _ell = _guest
                        .firstChildElement("arch")
                        .firstChildElement("machine");
            };
            while ( !_ell.isNull() ) {
                if ( _ell.firstChild().toText().data().compare(_machine)==0 ) {
                    _vcpu = _ell.attribute("maxCpus");
                    break;
                };
                _ell = _ell.nextSiblingElement("machine");
            };
            break;
        };
        _guest = _guest.nextSiblingElement("guest");
    };
    //qDebug()<<_vcpu;
    //s<<"MAX VCPU :"<<_vcpu<<endl;
    emit maxVCPU(_vcpu);
}
void _Arch::archChanged(const QString &_arch)
{
    //QTextStream s(stdout);
    //s<<"arch changed :"<< _arch<<endl;
    QString _domType, _osType, _emulType;
    machines->clear();
    QDomElement _guest = doc
            .firstChildElement("capabilities")
            .firstChildElement("guest");
    while ( !_guest.isNull() ) {
        _osType = _guest
                .firstChildElement("os_type")
                .firstChild().toText().data();
        if ( _guest
             .firstChildElement("arch")
             .attribute("name").compare(_arch)==0 ) {
            QDomElement _domain, _ell;
            _domain = _guest
                    .firstChildElement("arch")
                    .firstChildElement("domain");
            bool exist = false;
            /*
            while ( !_domain.isNull() ) {
                _domType = _domain.attribute("type");
                _emulType = _domain
                        .firstChildElement("emulator")
                        .firstChild().toText().data();
                // WARNING: in this application
                // KVM has the advantage over QEMU
                if ( _domType.compare("kvm")==0 ) {
                    _ell = _domain
                            .firstChildElement("machine");
                    exist = true;
                    break;
                };
                _domain = _domain.nextSiblingElement("domain");
            };
            */
            if ( !exist ) {
                _emulType = _guest
                        .firstChildElement("arch")
                        .firstChildElement("emulator")
                        .firstChild().toText().data();
                _ell = _guest
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
        _guest = _guest.nextSiblingElement("guest");
    };
    emit domainType(_domType);
    emit archType(_arch);
    emit osType(_osType);
    emit emulatorType(_emulType);
}
