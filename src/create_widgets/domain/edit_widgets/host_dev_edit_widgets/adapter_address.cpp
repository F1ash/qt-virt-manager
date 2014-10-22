#include "adapter_address.h"

AdapterAddress::AdapterAddress(QWidget *parent) :
    QWidget(parent)
{
    adapterAddr = new QCheckBox("SCSI Device\n address:", this);
    busLabel = new QLabel("Bus:", this);
    targetLabel = new QLabel("Target:", this);
    unitLabel = new QLabel("Unit:", this);
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
            this, SIGNAL(dataChanged()));
    connect(bus, SIGNAL(valueChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(target, SIGNAL(valueChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(unit, SIGNAL(valueChanged(int)),
            this, SIGNAL(dataChanged()));
}
