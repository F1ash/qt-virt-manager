#include "net_interface.h"

NetInterface::NetInterface(QWidget *parent, virNetworkPtr *nets) :
    _QWidget(parent), existNetwork(nets)
{
    setObjectName("Network:Device");
    useExistNetwork = new QCheckBox("Use Exist Network", this);
    networks = new QComboBox(this);
    int i = 0;
    while ( existNetwork[i] != NULL ) {
        networks->addItem( QString( virNetworkGetName(existNetwork[i]) ) );
        i++;
    };
    netDescWdg = new CreateVirtNetwork(this);
    scroll = new QScrollArea(this);
    scroll->setWidget(netDescWdg);
    commonLayout = new QVBoxLayout();
    commonLayout->addWidget(useExistNetwork);
    commonLayout->addWidget(networks);
    commonLayout->addWidget(scroll);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
    connect(useExistNetwork, SIGNAL(toggled(bool)), this, SLOT(changeUsedNetwork(bool)));
    connect(networks, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeUsedNetwork(QString)));
    useExistNetwork->setChecked(true);
}
NetInterface::~NetInterface()
{
    disconnect(useExistNetwork, SIGNAL(toggled(bool)), this, SLOT(changeUsedNetwork(bool)));
    disconnect(networks, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeUsedNetwork(QString)));
    int i = 0;
    while ( existNetwork[i] != NULL ) {
        virNetworkFree(existNetwork[i]);
        i++;
    };
    // free useExistNetwork into VirtDomainControl widget
    delete useExistNetwork;
    useExistNetwork = 0;
    delete networks;
    networks = 0;

    delete netDescWdg;
    netDescWdg = 0;
    delete scroll;
    scroll = 0;
    delete commonLayout;
    commonLayout = 0;
}

/* public slots */


/* private slots */
void NetInterface::changeUsedNetwork(bool state)
{
    if (state) {
        networks->setEnabled(true);
        netDescWdg->setEnabled(false);
    } else  {
        networks->setEnabled(false);
        netDescWdg->setEnabled(true);
    };
}
void NetInterface::changeUsedNetwork(QString item)
{
    qDebug()<<item;
}
