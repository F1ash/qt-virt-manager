#include "bios_boot.h"

BIOS_Boot::BIOS_Boot(QWidget *parent, QString _caps) :
    _QWidget(parent), capabilities(_caps)
{
    architecture = new _Arch(this, capabilities);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(architecture);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
    connect(architecture, SIGNAL(domainType(QString&)),
            this, SIGNAL(domainType(QString&)));
    connect(architecture, SIGNAL(osType(QString&)),
            this, SIGNAL(osType(QString&)));
    connect(architecture, SIGNAL(emulatorType(QString&)),
            this, SIGNAL(emulatorType(QString&)));
    architecture->setItems();
}

/* public slots */
void BIOS_Boot::setInitState()
{
    if ( this->isEnabled() ) architecture->setItems();
}
