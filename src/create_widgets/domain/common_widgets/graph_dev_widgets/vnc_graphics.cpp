#include "vnc_graphics.h"

#define KEYMAPs QStringList()<<"auto"<<"en-gb"<<"en-us"<<"ru"<<"fr"<<"de"<<"is"<<"it"<<"ja"

VNC_Graphics::VNC_Graphics(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    readNetworkList();
    addrLabel = new QLabel("Address:", this);
    address = new QComboBox(this);
    address->setEditable(false);
    address->addItem("HyperVisor default", "");
    address->addItem("LocalHost only", "127.0.0.1");
    address->addItem("All Interfaces", "0.0.0.0");
    address->addItem("Custom", "custom");
    address->addItem("Named configured Network", "network");
    address->insertSeparator(4);
    networks = new QComboBox(this);
    networks->setVisible(false);
    networks->addItems(nets);
    autoPort = new QCheckBox("AutoPort", this);
    port = new QSpinBox(this);
    port->setRange(10, 65535);
    port->setValue(5900);
    port->setEnabled(false);
    usePassw = new QCheckBox("Password", this);
    passw = new QLineEdit(this);
    passw->setEnabled(false);
    keymapLabel = new QLabel("Keymap:", this);
    keymap = new QComboBox(this);
    keymap->setEditable(true);
    keymap->addItems(KEYMAPs);
    keymap->setEnabled(false);
    shareLabel = new QLabel("Share policy:", this);
    sharePolicy = new QComboBox(this);
    sharePolicy->addItem("Multiple clients (default)", "");
    sharePolicy->addItem("Exclusive access", "allow-exclusive");
    sharePolicy->addItem("Disable exclusive client access", "force-shared");
    sharePolicy->addItem("Welcomes every connection unconditionally", "ignore");
    commonLayout = new QGridLayout();
    commonLayout->addWidget(addrLabel, 0, 0);
    commonLayout->addWidget(address, 0, 1);
    commonLayout->addWidget(networks, 1, 1);
    commonLayout->addWidget(autoPort, 2, 0);
    commonLayout->addWidget(port, 2, 1);
    commonLayout->addWidget(usePassw, 3, 0);
    commonLayout->addWidget(passw, 3, 1);
    commonLayout->addWidget(keymapLabel, 4, 0);
    commonLayout->addWidget(keymap, 4, 1);
    commonLayout->addWidget(shareLabel, 5, 0, Qt::AlignTop);
    commonLayout->addWidget(sharePolicy, 5, 1, Qt::AlignTop);
    setLayout(commonLayout);
    connect(address, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(addressEdit(QString)));
    connect(autoPort, SIGNAL(toggled(bool)),
            this, SLOT(usePort(bool)));
    connect(usePassw, SIGNAL(toggled(bool)),
            this, SLOT(usePassword(bool)));
    autoPort->setChecked(true);
    usePassw->setChecked(false);
    // dataChanged connections
    connect(address, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(networks, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(autoPort, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(port, SIGNAL(valueChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(usePassw, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(passw, SIGNAL(textEdited(QString)),
            this, SIGNAL(dataChanged()));
    connect(keymap, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(sharePolicy, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
}

/* public slots */
QDomDocument VNC_Graphics::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _listen, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("graphics");
    _devDesc.setAttribute("type", "vnc");
    QString _sharePolicy = sharePolicy->itemData(
                sharePolicy->currentIndex(), Qt::UserRole).toString();
    if ( !_sharePolicy.isEmpty() )
        _devDesc.setAttribute("sharePolicy", _sharePolicy);
    if ( autoPort->isChecked() ) {
        _devDesc.setAttribute("autoport", "yes");
    } else {
        _devDesc.setAttribute("port", port->text());
    };
    if ( usePassw->isChecked() ) {
        _devDesc.setAttribute("passwd", passw->text());
        _devDesc.setAttribute("keymap", keymap->currentText());
    };
    QString _address = address->itemData(
                address->currentIndex(), Qt::UserRole).toString();
    if ( !_address.isEmpty() && _address!="network" ) {
        _listen = doc.createElement("listen");
        _listen.setAttribute("type", "address");
        if ( _address!="custom" ) {
            _listen.setAttribute("address", _address);
        } else {
            _listen.setAttribute(
                        "address",
                        address->currentText());
        };
        _devDesc.appendChild(_listen);
    } else if ( _address=="network" && networks->count()>0 ) {
            _listen = doc.createElement("listen");
            _listen.setAttribute("type", "network");
            _listen.setAttribute("network", networks->currentText());
            _devDesc.appendChild(_listen);
    };
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void VNC_Graphics::setDataDescription(QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    QDomElement _device, _listen;
    doc.setContent(_xmlDesc);
    _device = doc.firstChildElement("device")
            .firstChildElement("graphics");
    QString _sharePolicy = _device.attribute("sharePolicy");
    int idx = sharePolicy->findData(
                _sharePolicy,
                Qt::UserRole,
                Qt::MatchContains);
    sharePolicy->setCurrentIndex( (idx<0)? 0:idx );
    autoPort->setChecked(
                _device.attribute("autoport")=="yes");
    if ( !autoPort->isChecked() )
        port->setValue(
                    _device.attribute("port").toInt());
    usePassw->setChecked( _device.hasAttribute("passwd") );
    if ( _device.hasAttribute("passwd") ) {
        QString _password = _device.attribute("passwd");
        passw->setText(_password);
        idx = keymap->findText(
                    _device.attribute("keymap"),
                    Qt::MatchContains);
        keymap->setCurrentIndex( (idx<0)? 0:idx );
    };
    _listen = _device.firstChildElement("listen");
    if ( !_listen.isNull() ) {
        QString _type, _data;
        _type = _listen.attribute("type");
        _data = _listen.attribute(_type);
        if ( !_type.isEmpty() ) {
            idx = address->findData(
                        _type,
                        Qt::UserRole,
                        Qt::MatchContains);
            address->setCurrentIndex( (idx<0)? 3:idx );
            if ( _type=="address" ) {
                if ( address->currentIndex()==3 )
                    address->setEditText(_data);
            } else if ( _type=="network" ) {
                idx = networks->findText(
                            _data,
                            Qt::MatchContains);
                networks->setCurrentIndex( (idx<0)? 0:idx );
            } else
                address->setCurrentIndex(0);
        } else
            address->setCurrentIndex(0);
    } else
        address->setCurrentIndex(0);
}

/* private slots */
void VNC_Graphics::usePort(bool state)
{
    port->setEnabled(!state);
}
void VNC_Graphics::usePassword(bool state)
{
    passw->setEnabled(state);
    keymap->setEnabled(state);
}
void VNC_Graphics::addressEdit(QString s)
{
    if ( s.contains("Network") ) {
        address->setEditable(false);
        addrLabel->setText("Network:");
        networks->setVisible(true);
    } else {
        addrLabel->setText("Address:");
        networks->setVisible(false);
        if ( s == "Custom" ) {
            address->setEditable(true);
            address->clearEditText();
        } else {
            address->setEditable(false);
        }
    }
}
void VNC_Graphics::readNetworkList()
{
    virNetworkPtr *networks = NULL;
    unsigned int flags = VIR_CONNECT_LIST_NETWORKS_ACTIVE |
                         VIR_CONNECT_LIST_NETWORKS_INACTIVE;
    int ret = virConnectListAllNetworks(currWorkConnect, &networks, flags);
    if ( ret<0 ) {
        sendConnErrors();
    } else {
        int i = 0;
        while ( networks[i] != NULL ) {
            nets.append( virNetworkGetName(networks[i]) );
            virNetworkFree(networks[i]);
            i++;
        };
    };
    free(networks);
}

void VNC_Graphics::sendConnErrors()
{
    virtErrors = virConnGetLastError(currWorkConnect);
    if ( virtErrors!=NULL && virtErrors->code>0 ) {
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                       .arg(QString().fromUtf8(virtErrors->message)) );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void VNC_Graphics::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL && virtErrors->code>0 )
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                       .arg(QString().fromUtf8(virtErrors->message)) );
    virResetLastError();
}
