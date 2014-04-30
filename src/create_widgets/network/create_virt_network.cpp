#include "create_virt_network.h"

CreateVirtNetwork::CreateVirtNetwork(QWidget *parent) :
    _QWidget(parent)
{
    bridgeLayout = new QGridLayout();
    bridgeName = new QLineEdit(this);
    bridgeName->setPlaceholderText("Enter bridge name");
    stp = new QCheckBox("Spanning Tree Protocol", this);
    stp->setChecked(true); // default state
    delayLabel = new QLabel("Delay", this);
    delay = new QSpinBox(this);
    delay->setRange(0, 360);
    bridgeLayout->addWidget(bridgeName, 0, 0, 1, 2);
    bridgeLayout->addWidget(stp, 2, 0, 3, 2);
    bridgeLayout->addWidget(delayLabel, 4, 0, 5, 1);
    bridgeLayout->addWidget(delay, 4, 1, 5, 2);
    bridge = new QWidget(this);
    bridge->setLayout(bridgeLayout);

    netDescLayout = new QVBoxLayout();
    networkName = new QLineEdit(this);
    networkName->setPlaceholderText("Enter Network Name");
    uuid = new QLabel("UUID:");
    netDescLayout->addWidget(networkName);
    netDescLayout->addWidget(uuid);
    netDescLayout->addWidget(bridge);
    setLayout(netDescLayout);
}
CreateVirtNetwork::~CreateVirtNetwork()
{
    delete networkName;
    networkName = 0;
    delete uuid;
    uuid = 0;
    delete bridgeName;
    bridgeName = 0;
    delete stp;
    stp = 0;
    delete delayLabel;
    delayLabel = 0;
    delete delay;
    delay = 0;
    delete bridgeLayout;
    bridgeLayout = 0;
    delete bridge;
    bridge = 0;
    delete netDescLayout;
    netDescLayout = 0;
}
