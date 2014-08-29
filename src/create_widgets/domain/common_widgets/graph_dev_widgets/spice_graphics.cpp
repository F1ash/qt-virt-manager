#include "spice_graphics.h"

#define KEYMAPs QStringList()<<"auto"<<"en-gb"<<"en-us"<<"ru"<<"fr"<<"de"<<"is"<<"it"<<"ja"

Spice_Graphics::Spice_Graphics(
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
    tlsPortLabel = new QCheckBox("Use TLS", this);
    tlsPort = new QSpinBox(this);
    tlsPort->setRange(10, 65535);
    tlsPort->setValue(5901);
    tlsPort->setEnabled(false);
    usePassw = new QCheckBox("Password", this);
    passw = new QLineEdit(this);
    passw->setEnabled(false);
    keymapLabel = new QLabel("Keymap:", this);
    keymap = new QComboBox(this);
    keymap->setEditable(true);
    keymap->addItems(KEYMAPs);
    keymap->setEnabled(false);
    defaultLabel = new QLabel("Default policy:", this);
    defaultPolicy = new QComboBox(this);
    defaultPolicy->addItem("any");
    defaultPolicy->addItem("secure");
    defaultPolicy->addItem("insecure");
    mainLabel = new QCheckBox("Main:", this);
    main = new QComboBox(this);
    main->addItem("any");
    main->addItem("secure");
    main->addItem("insecure");
    main->setEnabled(false);
    displayLabel = new QCheckBox("Display:", this);
    display = new QComboBox(this);
    display->addItem("any");
    display->addItem("secure");
    display->addItem("insecure");
    display->setEnabled(false);
    inputsLabel = new QCheckBox("Inputs:", this);
    inputs = new QComboBox(this);
    inputs->addItem("any");
    inputs->addItem("secure");
    inputs->addItem("insecure");
    inputs->setEnabled(false);
    cursorLabel = new QCheckBox("Cursor:", this);
    cursor = new QComboBox(this);
    cursor->addItem("any");
    cursor->addItem("secure");
    cursor->addItem("insecure");
    cursor->setEnabled(false);
    playbackLabel = new QCheckBox("Playback:", this);
    playback = new QComboBox(this);
    playback->addItem("any");
    playback->addItem("secure");
    playback->addItem("insecure");
    playback->setEnabled(false);
    recordLabel = new QCheckBox("Record:", this);
    record = new QComboBox(this);
    record->addItem("any");
    record->addItem("secure");
    record->addItem("insecure");
    record->setEnabled(false);
    smartcardLabel = new QCheckBox("Smartcard:", this);
    smartcard = new QComboBox(this);
    smartcard->addItem("any");
    smartcard->addItem("secure");
    smartcard->addItem("insecure");
    smartcard->setEnabled(false);
    usbredirLabel = new QCheckBox("Usbredir:", this);
    usbredir = new QComboBox(this);
    usbredir->addItem("any");
    usbredir->addItem("secure");
    usbredir->addItem("insecure");
    usbredir->setEnabled(false);
    elementsLayout = new QGridLayout();
    elementsLayout->addWidget(mainLabel, 1, 0);
    elementsLayout->addWidget(main, 1, 1);
    elementsLayout->addWidget(displayLabel, 2, 0);
    elementsLayout->addWidget(display, 2, 1);
    elementsLayout->addWidget(inputsLabel, 3, 0);
    elementsLayout->addWidget(inputs, 3, 1);
    elementsLayout->addWidget(cursorLabel, 4, 0);
    elementsLayout->addWidget(cursor, 4, 1);
    elementsLayout->addWidget(playbackLabel, 5, 0);
    elementsLayout->addWidget(playback, 5, 1);
    elementsLayout->addWidget(recordLabel, 6, 0);
    elementsLayout->addWidget(record, 6, 1);
    elementsLayout->addWidget(smartcardLabel, 7, 0);
    elementsLayout->addWidget(smartcard, 7, 1);
    elementsLayout->addWidget(usbredirLabel, 8, 0, Qt::AlignTop);
    elementsLayout->addWidget(usbredir, 8, 1, Qt::AlignTop);
    elements = new QWidget(this);
    elements->setLayout(elementsLayout);
    elements->setEnabled(false);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(addrLabel, 0, 0);
    commonLayout->addWidget(address, 0, 1);
    commonLayout->addWidget(networks, 1, 1);
    commonLayout->addWidget(autoPort, 2, 0);
    commonLayout->addWidget(port, 2, 1);
    commonLayout->addWidget(tlsPortLabel, 3, 0);
    commonLayout->addWidget(tlsPort, 3, 1);
    commonLayout->addWidget(usePassw, 4, 0);
    commonLayout->addWidget(passw, 4, 1);
    commonLayout->addWidget(keymapLabel, 5, 0);
    commonLayout->addWidget(keymap, 5, 1);
    commonLayout->addWidget(defaultLabel, 6, 0);
    commonLayout->addWidget(defaultPolicy, 6, 1);
    commonLayout->addWidget(elements, 7, 0, 8, 2, Qt::AlignTop);
    setLayout(commonLayout);
    connect(address, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(addressEdit(QString)));
    connect(autoPort, SIGNAL(toggled(bool)),
            this, SLOT(useAutoPort(bool)));
    connect(tlsPortLabel, SIGNAL(toggled(bool)),
            this, SLOT(useTLSPort(bool)));
    connect(usePassw, SIGNAL(toggled(bool)),
            this, SLOT(usePassword(bool)));
    autoPort->setChecked(true);
    usePassw->setChecked(false);
    connect(defaultPolicy, SIGNAL(currentIndexChanged(int)),
            this, SLOT(defaultPolicyChanged(int)));
    connect(mainLabel, SIGNAL(toggled(bool)),
            main, SLOT(setEnabled(bool)));
    connect(displayLabel, SIGNAL(toggled(bool)),
            display, SLOT(setEnabled(bool)));
    connect(inputsLabel, SIGNAL(toggled(bool)),
            inputs, SLOT(setEnabled(bool)));
    connect(cursorLabel, SIGNAL(toggled(bool)),
            cursor, SLOT(setEnabled(bool)));
    connect(playbackLabel, SIGNAL(toggled(bool)),
            playback, SLOT(setEnabled(bool)));
    connect(recordLabel, SIGNAL(toggled(bool)),
            record, SLOT(setEnabled(bool)));
    connect(smartcardLabel, SIGNAL(toggled(bool)),
            smartcard, SLOT(setEnabled(bool)));
    connect(usbredirLabel, SIGNAL(toggled(bool)),
            usbredir, SLOT(setEnabled(bool)));
}

/* public slots */
QDomDocument Spice_Graphics::getDevDocument() const
{
    QDomDocument doc;
    QDomElement _listen, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("graphics");
    _devDesc.setAttribute("type", "spice");
    if ( tlsPortLabel->isChecked() ) _devDesc.setAttribute("tlsPort", tlsPort->text());
    _devDesc.setAttribute("defaultPolicy", defaultPolicy->currentText());
    if ( autoPort->isChecked() ) {
        _devDesc.setAttribute("autoport", "yes");
    } else {
        _devDesc.setAttribute("port", port->text());
    };
    if ( usePassw->isChecked() ) {
        _devDesc.setAttribute("passwd", passw->text());
        _devDesc.setAttribute("keymap", keymap->currentText());
    };
    if ( elements->isEnabled() ) {
        if ( mainLabel->isChecked() ) {
            QDomElement _main = doc.createElement("channel");
            _main.setAttribute("name", "main");
            _main.setAttribute("mode", main->currentText());
            _devDesc.appendChild(_main);
        };
        if ( displayLabel->isChecked() ) {
            QDomElement _display = doc.createElement("channel");
            _display.setAttribute("name", "display");
            _display.setAttribute("mode", display->currentText());
            _devDesc.appendChild(_display);
        };
        if ( inputsLabel->isChecked() ) {
            QDomElement _inputs = doc.createElement("channel");
            _inputs.setAttribute("name", "inputs");
            _inputs.setAttribute("mode", inputs->currentText());
            _devDesc.appendChild(_inputs);
        };
        if ( cursorLabel->isChecked() ) {
            QDomElement _cursor = doc.createElement("channel");
            _cursor.setAttribute("name", "cursor");
            _cursor.setAttribute("mode", cursor->currentText());
            _devDesc.appendChild(_cursor);
        };
        if ( playbackLabel->isChecked() ) {
            QDomElement _playback = doc.createElement("channel");
            _playback.setAttribute("name", "playback");
            _playback.setAttribute("mode", playback->currentText());
            _devDesc.appendChild(_playback);
        };
        if ( recordLabel->isChecked() ) {
            QDomElement _record = doc.createElement("channel");
            _record.setAttribute("name", "record");
            _record.setAttribute("mode", record->currentText());
            _devDesc.appendChild(_record);
        };
        if ( smartcardLabel->isChecked() ) {
            QDomElement _smartcard = doc.createElement("channel");
            _smartcard.setAttribute("name", "smartcard");
            _smartcard.setAttribute("mode", smartcard->currentText());
            _devDesc.appendChild(_smartcard);
        };
        if ( usbredirLabel->isChecked() ) {
            QDomElement _usbredir = doc.createElement("channel");
            _usbredir.setAttribute("name", "usbredir");
            _usbredir.setAttribute("mode", usbredir->currentText());
            _devDesc.appendChild(_usbredir);
        };
    };
    QString _address = address->itemData(address->currentIndex(), Qt::UserRole).toString();
    if ( !_address.isEmpty() && _address!="network" ) {
        _listen = doc.createElement("listen");
        _listen.setAttribute("type", "address");
        if ( _address.isEmpty() ) {
            _listen.setAttribute("address", address->currentText());
        } else {
            _listen.setAttribute("address", _address);
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

/* private slots */
void Spice_Graphics::useAutoPort(bool state)
{
    port->setEnabled(!state);
    elementsSetRequired( !state && tlsPortLabel->isChecked()
                         && defaultPolicy->currentIndex()>0 );
}
void Spice_Graphics::useTLSPort(bool state)
{
    tlsPort->setEnabled(state);
    elementsSetRequired( state && !autoPort->isChecked()
                         && defaultPolicy->currentIndex()>0 );
}
void Spice_Graphics::usePassword(bool state)
{
    passw->setEnabled(state);
    keymap->setEnabled(state);
}
void Spice_Graphics::addressEdit(QString s)
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
void Spice_Graphics::elementsSetRequired(bool state)
{
    elements->setEnabled(state);
}
void Spice_Graphics::defaultPolicyChanged(int i)
{
    if ( !elements->isEnabled() ) {
        main->setCurrentIndex(i);
        display->setCurrentIndex(i);
        inputs->setCurrentIndex(i);
        cursor->setCurrentIndex(i);
        playback->setCurrentIndex(i);
        record->setCurrentIndex(i);
        smartcard->setCurrentIndex(i);
        usbredir->setCurrentIndex(i);
    };
    elementsSetRequired( tlsPortLabel->isChecked()
                         && !autoPort->isChecked()
                         && defaultPolicy->currentIndex()>0 );
}
void Spice_Graphics::readNetworkList()
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

void Spice_Graphics::sendConnErrors()
{
    virtErrors = virConnGetLastError(currWorkConnect);
    if ( virtErrors!=NULL && virtErrors->code>0 ) {
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                       .arg(QString().fromUtf8(virtErrors->message)) );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void Spice_Graphics::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL && virtErrors->code>0 )
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                       .arg(QString().fromUtf8(virtErrors->message)) );
    virResetLastError();
}
