#include "virtual_network.h"

Virtual_Network::Virtual_Network(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    networkLabel = new QLabel("Network:", this);
    network = new QComboBox(this);
    targetLabel = new QLabel("Target Device name:", this);
    target = new QLineEdit(this);
    target->setPlaceholderText("vnetN");
    baseLayout = new QGridLayout();
    baseLayout->addWidget(networkLabel, 0, 0);
    baseLayout->addWidget(network, 0, 1);
    baseLayout->addWidget(targetLabel, 1, 0);
    baseLayout->addWidget(target, 1, 1);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    mac = new MAC_Address(this);
    virtPort = new VirtualPort(this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(mac);
    commonLayout->addWidget(virtPort);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(network, SIGNAL(currentIndexChanged(int)),
            this, SLOT(networkChanged(int)));
    // set empty type for the general case
    //virtPort->type->setCurrentIndex( virtPort->type->findText("802.1Qbh") );
    setAvailableVirtNetworks();
    // dataChanged connections
    connect(network, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(network, SIGNAL(editTextChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(target, SIGNAL(textEdited(QString)),
            this, SIGNAL(dataChanged()));
    connect(mac, SIGNAL(dataChanged()),
            this, SIGNAL(dataChanged()));
    connect(virtPort, SIGNAL(dataChanged()),
            this, SIGNAL(dataChanged()));
}

/* public slots */
QDomDocument Virtual_Network::getDataDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _target, _mac, _virtualport,
            _parameters, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("interface");
    _source = doc.createElement("source");
    QString _network = network->itemData(network->currentIndex(), Qt::UserRole).toString();
    if ( _network.isEmpty() ) _network = network->currentText();
    _source.setAttribute("network", _network);
    _devDesc.appendChild(_source);
    if ( !target->text().isEmpty() ) {
        _target = doc.createElement("target");
        _target.setAttribute("dev", target->text());
        _devDesc.appendChild(_target);
    };
    if ( !mac->getMACAddress().isEmpty() ) {
        _mac = doc.createElement("mac");
        _mac.setAttribute("address", mac->getMACAddress());
        _devDesc.appendChild(_mac);
    };

    ParameterList p = virtPort->getParameterList();
    if ( !p.isEmpty() ) {
        _virtualport = doc.createElement("virtualport");
        _parameters = doc.createElement("parameters");
        foreach (QString key, p.keys()) {
            if ( !key.isEmpty() ) {
                if ( key=="type" ) {
                    _virtualport.setAttribute(key, p.value(key));
                } else
                    _parameters.setAttribute(key, p.value(key));
            };
            _virtualport.appendChild(_parameters);
        };
        _devDesc.appendChild(_virtualport);
    };
    _devDesc.setAttribute("type", "network");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void Virtual_Network::setDataDescription(QString &xmlDesc)
{
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _source, _target, _mac, _virtport;
    _device = doc.firstChildElement("device")
            .firstChildElement("interface");
    _source = _device.firstChildElement("source");
    _target = _device.firstChildElement("target");
    _mac = _device.firstChildElement("mac");
    _virtport = _device.firstChildElement("virtualport");
    QString _attr;
    _attr = _source.attribute("network");
    int idx = network->findText(_attr, Qt::MatchContains);
    idx = (idx<0)? network->count()-1:idx;
    network->setCurrentIndex( idx );
    if ( idx==network->count()-1 ) {
        network->setEditText(_attr);
    };
    _attr = _target.attribute("dev");
    target->setText(_attr);
    mac->setUsage(!_mac.isNull());
    if ( !_mac.isNull() ) {
        _attr = _mac.attribute("address");
        mac->setMACAddress(_attr);
    };
    virtPort->setUsage( !_virtport.isNull() );
    if ( !_virtport.isNull() ) {
        ParameterList _list;
        _list.insert("type", _virtport.attribute("type"));
        QDomElement _params = _virtport.firstChildElement("parameters");
        if ( _params.hasAttribute("managerid") ) {
            _list.insert("managerid", _params.attribute("managerid"));
        };
        if ( _params.hasAttribute("typeid") ) {
            _list.insert("typeid", _params.attribute("typeid"));
        };
        if ( _params.hasAttribute("typeidver") ) {
            _list.insert("typeidver", _params.attribute("typeidver"));
        };
        if ( _params.hasAttribute("instanceId") ) {
            _list.insert("instanceid", _params.attribute("instanceid"));
        };
        if ( _params.hasAttribute("interfaceid") ) {
            _list.insert("interfaceid", _params.attribute("interfaceid"));
        };
        if ( _params.hasAttribute("profileid") ) {
            _list.insert("profileid", _params.attribute("profileid"));
        };
        virtPort->setParameterList(_list);
    };
}

/* private slots */
void Virtual_Network::networkChanged(int i)
{
    QString _network = network->itemData(i, Qt::UserRole).toString();
    if ( !_network.isEmpty() ) {
        network->setEditable(false);
    } else {
        network->setEditable(true);
    };
    network->clearEditText();
}
void Virtual_Network::setAvailableVirtNetworks()
{
    virNetworkPtr *networks = NULL;
    unsigned int flags = VIR_CONNECT_LIST_NETWORKS_ACTIVE |
                         VIR_CONNECT_LIST_NETWORKS_INACTIVE;
    int ret = virConnectListAllNetworks(currWorkConnect, &networks, flags);
    if ( ret<0 ) {
        // if failed, then set to default virtual network
        network->addItem("VirtNetwork detect failed", "default");
    } else {
        int i = 0;
        while ( networks[i] != NULL ) {
            QString _network = QString( virNetworkGetName(networks[i]) );
            network->addItem(
                        QString("Virtual network '%1'")
                        .arg(_network),
                        _network);
            virNetworkFree(networks[i]);
            i++;
        };
    };
    free(networks);
    network->insertSeparator(network->count());
    network->addItem("Set exist Virtual Network manually", "");
}
