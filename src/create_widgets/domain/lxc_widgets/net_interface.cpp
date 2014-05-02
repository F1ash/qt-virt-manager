#include "net_interface.h"

LXC_NetInterface::LXC_NetInterface(QWidget *parent, virNetworkPtr *nets) :
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

    bridgeName = new QLineEdit(this);
    bridgeName->setPlaceholderText("Enter Exist Virtual Bridge Name");
    mac = new QLineEdit(this);
    mac->setPlaceholderText("00:11:22:33:44:55 (optional)");
    macLabel = new QLabel("MAC:", this);
    netDescLayout = new QGridLayout();
    netDescLayout->addWidget(bridgeName, 0, 0, 1, 2);
    netDescLayout->addWidget(macLabel, 2, 0, Qt::AlignLeft);
    netDescLayout->addWidget(mac, 2, 1);
    netDescWdg = new QWidget(this);
    netDescWdg->setLayout(netDescLayout);
    commonLayout = new QVBoxLayout();
    commonLayout->addWidget(useExistNetwork);
    commonLayout->addWidget(networks);
    commonLayout->addWidget(netDescWdg);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
    connect(useExistNetwork, SIGNAL(toggled(bool)), this, SLOT(changeUsedNetwork(bool)));
    connect(networks, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeUsedNetwork(QString)));
    useExistNetwork->setChecked(true);
}
LXC_NetInterface::~LXC_NetInterface()
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

    delete bridgeName;
    bridgeName = 0;
    delete macLabel;
    macLabel = 0;
    delete mac;
    mac = 0;
    delete netDescLayout;
    delete netDescWdg;
    netDescWdg = 0;
    delete commonLayout;
    commonLayout = 0;
}

/* public slots */
QDomNodeList LXC_NetInterface::getNodeList() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _interface, _source, _mac;
    if ( !useExistNetwork->isChecked() ) {
        _interface= doc.createElement("interface");
        _interface.setAttribute("type", "bridge");
        doc.appendChild(_interface);

        _source= doc.createElement("source");
        _source.setAttribute("bridge", bridgeName->text());
        _interface.appendChild(_source);

        if ( !mac->text().isEmpty() ) {
            _mac= doc.createElement("mac");
            _mac.setAttribute("address", mac->text());
            _interface.appendChild(_mac);
        };
    } else {
        _interface= doc.createElement("interface");
        _interface.setAttribute("type", "network");
        doc.appendChild(_interface);

        _source= doc.createElement("source");
        _source.setAttribute("network", networks->currentText());
        _interface.appendChild(_source);
    };

    //qDebug()<<doc.toString();
    return doc.childNodes();
}

/* private slots */
void LXC_NetInterface::changeUsedNetwork(bool state)
{
    if (state) {
        networks->setEnabled(true);
        netDescWdg->setEnabled(false);
    } else  {
        networks->setEnabled(false);
        netDescWdg->setEnabled(true);
    };
}
void LXC_NetInterface::changeUsedNetwork(QString item)
{
    qDebug()<<item;
}
