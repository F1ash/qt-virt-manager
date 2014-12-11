#include "nat_mode_widget.h"

NAT_Mode_widget::NAT_Mode_widget(QWidget *parent) :
    QWidget(parent)
{
    devLabel = new QLabel("Dev name:", this);
    dev = new QLineEdit(this);
    devLayout = new QHBoxLayout(this);
    devLayout->addWidget(devLabel);
    devLayout->addWidget(dev);
    addrRange = new QCheckBox("Use Address Range", this);
    ipStart = new QLineEdit(this);
    ipEnd = new QLineEdit(this);
    addrLayout = new QHBoxLayout();
    addrLayout->addWidget(ipStart);
    addrLayout->addWidget(ipEnd);
    portRange = new QCheckBox("Use Port Range", this);
    portStart = new QSpinBox(this);
    portStart->setRange(0, 65535);
    portEnd = new QSpinBox(this);
    portEnd->setRange(0, 65535);
    portLayout = new QHBoxLayout();
    portLayout->addWidget(portStart);
    portLayout->addWidget(portEnd);
    devWdg = new QWidget(this);
    devWdg->setLayout(devLayout);
    addrWdg = new QWidget(this);
    addrWdg->setLayout(addrLayout);
    addrWdg->setEnabled(false);
    portWdg = new QWidget(this);
    portWdg->setLayout(portLayout);
    portWdg->setEnabled(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(devWdg);
    commonLayout->addWidget(addrRange);
    commonLayout->addWidget(addrWdg);
    commonLayout->addWidget(portRange);
    commonLayout->addWidget(portWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(addrRange, SIGNAL(toggled(bool)),
            addrWdg, SLOT(setEnabled(bool)));
    connect(portRange, SIGNAL(toggled(bool)),
            portWdg, SLOT(setEnabled(bool)));
}
