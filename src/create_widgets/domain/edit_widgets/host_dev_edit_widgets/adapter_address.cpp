#include "adapter_address.h"

AdapterAddress::AdapterAddress(QWidget *parent) :
    QWidget(parent)
{
    adapterAddr = new QCheckBox(
                tr("SCSI Device\n address:"), this);
    busLabel = new QLabel(tr("Bus:"), this);
    targetLabel = new QLabel(tr("Target:"), this);
    unitLabel = new QLabel(tr("Unit:"), this);
    bus = new QSpinBox(this);
    target = new QSpinBox(this);
    unit = new QSpinBox(this);
    scsiAddrLayout = new QGridLayout();
    scsiAddrLayout->addWidget(busLabel, 0, 0);
    scsiAddrLayout->addWidget(bus, 0, 1);
    scsiAddrLayout->addWidget(targetLabel, 1, 0);
    scsiAddrLayout->addWidget(target, 1, 1);
    scsiAddrLayout->addWidget(unitLabel, 2, 0);
    scsiAddrLayout->addWidget(unit, 2, 1);
    scsiAddr = new QWidget(this);
    scsiAddr->setEnabled(false);
    scsiAddr->setLayout(scsiAddrLayout);
    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(adapterAddr);
    commonLayout->addWidget(scsiAddr);
    setLayout(commonLayout);
    connect(adapterAddr, SIGNAL(toggled(bool)),
            scsiAddr, SLOT(setEnabled(bool)));
    // dataChanged signals
    connect(adapterAddr, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(bus, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(target, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(unit, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
}

/* public slots */
void AdapterAddress::stateChanged()
{
    emit dataChanged();
}
