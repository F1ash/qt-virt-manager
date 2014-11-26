#include "cpu_topology.h"

CPU_Topology::CPU_Topology(QWidget *parent) :
    QWidget(parent)
{
    use = new QCheckBox("Use Topology", this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(use);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
}

/* public slots */
bool CPU_Topology::isUsed() const
{
    return use->isChecked();
}
void CPU_Topology::setUsage(bool state)
{
    use->setChecked(state);
}
