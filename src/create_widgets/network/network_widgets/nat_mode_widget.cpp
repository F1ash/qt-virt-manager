#include "nat_mode_widget.h"

NAT_Mode_widget::NAT_Mode_widget(QWidget *parent) :
    _QWidget(parent)
{
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
    addrWdg = new QWidget(this);
    addrWdg->setLayout(addrLayout);
    addrWdg->setEnabled(false);
    portWdg = new QWidget(this);
    portWdg->setLayout(portLayout);
    portWdg->setEnabled(false);
    commonLayout = new QVBoxLayout(this);
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
    connect(portStart, SIGNAL(valueChanged(int)),
            this, SLOT(portStartChanged(int)));
    connect(portEnd, SIGNAL(valueChanged(int)),
            this, SLOT(portEndChanged(int)));
}

/* private slots */
void NAT_Mode_widget::portStartChanged(int _value)
{
    if ( _value>portEnd->value() ) {
        portEnd->setValue(_value);
    };
}
void NAT_Mode_widget::portEndChanged(int _value)
{
    if ( _value<portStart->value() ) {
        portStart->setValue(_value);
    };
}
