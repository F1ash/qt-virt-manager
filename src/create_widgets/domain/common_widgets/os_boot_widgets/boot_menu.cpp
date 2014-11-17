#include "boot_menu.h"

BootMenu::BootMenu(QWidget *parent) :
    QWidget(parent)
{
    timeLabel = new QLabel("Timeout:", this);
    menu = new QCheckBox("BootMenu", this);
    timeOut = new QSpinBox(this);
    timeOut->setRange(0, 65535);
    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(menu);
    commonLayout->addSpacing(50);
    commonLayout->addWidget(timeLabel);
    commonLayout->addWidget(timeOut);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    // dataChanged connections
    connect(menu, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(timeOut, SIGNAL(valueChanged(int)),
            this, SIGNAL(dataChanged()));
}
