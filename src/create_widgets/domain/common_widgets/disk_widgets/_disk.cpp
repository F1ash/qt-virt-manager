#include "_disk.h"

_Disk::_Disk(
        QWidget *parent, virConnectPtr* connPtrPtr) :
    _QWidget(parent, connPtrPtr)
{
    baseLayout = new QGridLayout();
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    hosts = new _Hosts(this);
    hosts->setVisible(false);
    startupPolicy = new _StartupPolicy(this);
    devType = new _Device(this);
    target = new _Target(this);
    secLabels = new SecLabels(this);
    secLabels->setVisible(false);
    encrypt = new _UseEncryption(this, ptr_ConnPtr);
    encrypt->setVisible(false);
    connect(encrypt, SIGNAL(errorMsg(const QString&)),
            this, SIGNAL(errorMsg(const QString&)));
    readOnly = new _ReadOnly(this);
    addr = new DeviceAddress(this);
    addr->info->widget(2)->setEnabled(false);
    addr->info->widget(3)->setEnabled(false);
    addr->info->widget(4)->setEnabled(false);
    addr->info->widget(5)->setEnabled(false);
    addr->info->widget(6)->setEnabled(false);
    addr->info->widget(7)->setEnabled(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(hosts);
    commonLayout->addWidget(startupPolicy);
    commonLayout->addWidget(devType);
    commonLayout->addWidget(target);
    commonLayout->addWidget(secLabels);
    commonLayout->addWidget(encrypt);
    commonLayout->addWidget(readOnly);
    commonLayout->addWidget(addr);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    // dataChanged connections
    connect(hosts, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(target, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(devType, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(startupPolicy, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(encrypt, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(secLabels, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(readOnly, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}
