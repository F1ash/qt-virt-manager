#include "_disk.h"

_Disk::_Disk(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    baseLayout = new QGridLayout();
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    hosts = new _Hosts(this);
    hosts->setVisible(false);
    startupPolicy = new _StartupPolicy(this);
    devType = new _Device(this);
    target = new _Target(this);
    readOnly = new _ReadOnly(this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(hosts);
    commonLayout->addWidget(startupPolicy);
    commonLayout->addWidget(devType);
    commonLayout->addWidget(target);
    commonLayout->addWidget(readOnly);
    setLayout(commonLayout);
}

/* public slots */
QDomDocument _Disk::getDevDocument() const
{
    return QDomDocument();
}
