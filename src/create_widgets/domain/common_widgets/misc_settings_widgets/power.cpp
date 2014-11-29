#include "power.h"

Power::Power(QWidget *parent) :
    QWidget(parent)
{
    suspend_to_disk = new QCheckBox("Suspend_to_Disk", this);
    suspend_to_mem = new QCheckBox("Suspend_to_Memory", this);
    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(suspend_to_disk);
    commonLayout->addWidget(suspend_to_mem);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    // dataChanged connections
    connect(suspend_to_disk, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(suspend_to_mem, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
}
