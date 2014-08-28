#include "net_interface.h"

LXC_NetInterface::LXC_NetInterface(QWidget *parent, QStringList nets) :
    _QWidget(parent), existNetwork(nets)
{
    setObjectName("Network:Device");
    useExistNetwork = new QCheckBox("Use Exist Network", this);
    networks = new QComboBox(this);
    networks->addItems( nets );

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

/* public slots */
QDomDocument LXC_NetInterface::getDevDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _mac, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("interface");
    _source= doc.createElement("source");
    if ( !useExistNetwork->isChecked() ) {
        _source.setAttribute("bridge", bridgeName->text());

        if ( !mac->text().isEmpty() ) {
            _mac= doc.createElement("mac");
            _mac.setAttribute("address", mac->text());
            _devDesc.appendChild(_mac);
        };
    } else {
        _source.setAttribute("network", networks->currentText());
    };
    _devDesc.appendChild(_source);
    _devDesc.setAttribute("type", (useExistNetwork->isChecked())? "network" : "bridge");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    //qDebug()<<doc.toString();
    return doc;
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
