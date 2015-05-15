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
    keymapLabel = new QLabel("Keymap", this);
    keymap = new QComboBox(this);
    keymap->setEditable(true);
    keymap->addItems(KEYMAPs);
    keymap->setEnabled(false);
    defaultLabel = new QLabel("Default policy", this);
    defaultPolicy = new QComboBox(this);
    defaultPolicy->addItem("any");
    defaultPolicy->addItem("secure");
    defaultPolicy->addItem("insecure");
    baseLayout = new QGridLayout();
    baseLayout->addWidget(addrLabel, 0, 0);
    baseLayout->addWidget(address, 0, 1);
    baseLayout->addWidget(networks, 1, 1);
    baseLayout->addWidget(autoPort, 2, 0);
    baseLayout->addWidget(port, 2, 1);
    baseLayout->addWidget(tlsPortLabel, 3, 0);
    baseLayout->addWidget(tlsPort, 3, 1);
    baseLayout->addWidget(usePassw, 4, 0);
    baseLayout->addWidget(passw, 4, 1);
    baseLayout->addWidget(keymapLabel, 5, 0);
    baseLayout->addWidget(keymap, 5, 1);
    baseLayout->addWidget(defaultLabel, 6, 0);
    baseLayout->addWidget(defaultPolicy, 6, 1);
    baseElements = new QWidget(this);
    baseElements->setLayout(baseLayout);

    mainLabel = new QCheckBox("Main", this);
    mainLabel->setLayoutDirection(Qt::RightToLeft);
    main = new QComboBox(this);
    main->addItem("any");
    main->addItem("secure");
    main->addItem("insecure");
    main->setEnabled(false);
    displayLabel = new QCheckBox("Display", this);
    displayLabel->setLayoutDirection(Qt::RightToLeft);
    display = new QComboBox(this);
    display->addItem("any");
    display->addItem("secure");
    display->addItem("insecure");
    display->setEnabled(false);
    inputsLabel = new QCheckBox("Inputs", this);
    inputsLabel->setLayoutDirection(Qt::RightToLeft);
    inputs = new QComboBox(this);
    inputs->addItem("any");
    inputs->addItem("secure");
    inputs->addItem("insecure");
    inputs->setEnabled(false);
    cursorLabel = new QCheckBox("Cursor", this);
    cursorLabel->setLayoutDirection(Qt::RightToLeft);
    cursor = new QComboBox(this);
    cursor->addItem("any");
    cursor->addItem("secure");
    cursor->addItem("insecure");
    cursor->setEnabled(false);
    playbackLabel = new QCheckBox("Playback", this);
    playbackLabel->setLayoutDirection(Qt::RightToLeft);
    playback = new QComboBox(this);
    playback->addItem("any");
    playback->addItem("secure");
    playback->addItem("insecure");
    playback->setEnabled(false);
    recordLabel = new QCheckBox("Record", this);
    recordLabel->setLayoutDirection(Qt::RightToLeft);
    record = new QComboBox(this);
    record->addItem("any");
    record->addItem("secure");
    record->addItem("insecure");
    record->setEnabled(false);
    smartcardLabel = new QCheckBox("Smartcard", this);
    smartcardLabel->setLayoutDirection(Qt::RightToLeft);
    smartcard = new QComboBox(this);
    smartcard->addItem("any");
    smartcard->addItem("secure");
    smartcard->addItem("insecure");
    smartcard->setEnabled(false);
    usbredirLabel = new QCheckBox("Usbredir", this);
    usbredirLabel->setLayoutDirection(Qt::RightToLeft);
    usbredir = new QComboBox(this);
    usbredir->addItem("any");
    usbredir->addItem("secure");
    usbredir->addItem("insecure");
    usbredir->setEnabled(false);
    policyElementsLayout = new QGridLayout();
    policyElementsLayout->addWidget(mainLabel, 0, 0);
    policyElementsLayout->addWidget(main, 0, 1);
    policyElementsLayout->addWidget(displayLabel, 1, 0);
    policyElementsLayout->addWidget(display, 1, 1);
    policyElementsLayout->addWidget(inputsLabel, 2, 0);
    policyElementsLayout->addWidget(inputs, 2, 1);
    policyElementsLayout->addWidget(cursorLabel, 3, 0);
    policyElementsLayout->addWidget(cursor, 3, 1);
    policyElementsLayout->addWidget(playbackLabel, 4, 0);
    policyElementsLayout->addWidget(playback, 4, 1);
    policyElementsLayout->addWidget(recordLabel, 5, 0);
    policyElementsLayout->addWidget(record, 5, 1);
    policyElementsLayout->addWidget(smartcardLabel, 6, 0);
    policyElementsLayout->addWidget(smartcard, 6, 1);
    policyElementsLayout->addWidget(usbredirLabel, 7, 0, Qt::AlignTop);
    policyElementsLayout->addWidget(usbredir, 7, 1, Qt::AlignTop);
    policyElements = new QWidget(this);
    policyElements->setVisible(false);
    policyElements->setLayout(policyElementsLayout);

    compress = new QCheckBox("Compression", this);
    compressImage = new QCheckBox("Image", this);
    compressImage->setLayoutDirection(Qt::RightToLeft);
    compressJpeg = new QCheckBox("Jpeg", this);
    compressJpeg->setLayoutDirection(Qt::RightToLeft);
    compressZlib = new QCheckBox("Zlib", this);
    compressZlib->setLayoutDirection(Qt::RightToLeft);
    compressPlayback = new QCheckBox("Playback", this);
    compressPlayback->setLayoutDirection(Qt::RightToLeft);
    imageElement = new QComboBox(this);
    imageElement->setEnabled(false);
    imageElement->addItem("auto_glz");
    imageElement->addItem("auto_lz");
    imageElement->addItem("quic");
    imageElement->addItem("glz");
    imageElement->addItem("lz");
    imageElement->addItem("off");
    jpegElement = new QComboBox(this);
    jpegElement->setEnabled(false);
    jpegElement->addItem("auto");
    jpegElement->addItem("never");
    jpegElement->addItem("always");
    zlibElement = new QComboBox(this);
    zlibElement->setEnabled(false);
    zlibElement->addItem("auto");
    zlibElement->addItem("never");
    zlibElement->addItem("always");
    playbackElement = new QComboBox(this);
    playbackElement->setEnabled(false);
    playbackElement->addItem("on");
    playbackElement->addItem("off");
    compressElementsLayout = new QGridLayout();
    compressElementsLayout->addWidget(compressImage, 0, 0);
    compressElementsLayout->addWidget(imageElement, 0, 1);
    compressElementsLayout->addWidget(compressJpeg, 1, 0);
    compressElementsLayout->addWidget(jpegElement, 1, 1);
    compressElementsLayout->addWidget(compressZlib, 2, 0);
    compressElementsLayout->addWidget(zlibElement, 2, 1);
    compressElementsLayout->addWidget(compressPlayback, 3, 0);
    compressElementsLayout->addWidget(playbackElement, 3, 1);
    compressElements = new QWidget(this);
    compressElements->setVisible(false);
    compressElements->setLayout(compressElementsLayout);

    addition = new QCheckBox("Addition", this);
    streaming = new QCheckBox("Streaming", this);
    streaming->setLayoutDirection(Qt::RightToLeft);
    clipboard = new QCheckBox("Clipboard", this);
    clipboard->setLayoutDirection(Qt::RightToLeft);
    mouse = new QCheckBox("Mouse", this);
    mouse->setLayoutDirection(Qt::RightToLeft);
    filetransfer = new QCheckBox("File transfer", this);
    streamingElement = new QComboBox(this);
    streamingElement->setEnabled(false);
    streamingElement->addItem("filter");
    streamingElement->addItem("all");
    streamingElement->addItem("off");
    clipboardElement = new QComboBox(this);
    clipboardElement->setEnabled(false);
    //clipboardElement->addItem("yes");
    clipboardElement->addItem("no");
    mouseElement = new QComboBox(this);
    mouseElement->setEnabled(false);
    mouseElement->addItem("client");
    mouseElement->addItem("server");
    filetransferElement = new QComboBox(this);
    filetransferElement->setEnabled(false);
    //filetransferElement->addItem("yes");
    filetransferElement->addItem("no");
    additionElementsLayout = new QGridLayout();
    additionElementsLayout->addWidget(streaming, 0, 0);
    additionElementsLayout->addWidget(streamingElement, 0, 1);
    additionElementsLayout->addWidget(clipboard, 1, 0);
    additionElementsLayout->addWidget(clipboardElement, 1, 1);
    additionElementsLayout->addWidget(mouse, 2, 0);
    additionElementsLayout->addWidget(mouseElement, 2, 1);
    additionElementsLayout->addWidget(filetransfer, 3, 0);
    additionElementsLayout->addWidget(filetransferElement, 3, 1);
    additionElements = new QWidget(this);
    additionElements->setVisible(false);
    additionElements->setLayout(additionElementsLayout);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseElements);
    commonLayout->addWidget(policyElements);
    commonLayout->addWidget(compress);
    commonLayout->addWidget(compressElements);
    commonLayout->addWidget(addition, -1);
    commonLayout->addWidget(additionElements);
    commonLayout->addStretch(-1);
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
    connect(compress, SIGNAL(toggled(bool)),
            this, SLOT(compressStateChanged(bool)));
    connect(compress, SIGNAL(toggled(bool)),
            this, SLOT(compressStateChanged(bool)));
    connect(compressImage, SIGNAL(toggled(bool)),
            imageElement, SLOT(setEnabled(bool)));
    connect(compressJpeg, SIGNAL(toggled(bool)),
            jpegElement, SLOT(setEnabled(bool)));
    connect(compressZlib, SIGNAL(toggled(bool)),
            zlibElement, SLOT(setEnabled(bool)));
    connect(compressPlayback, SIGNAL(toggled(bool)),
            playbackElement, SLOT(setEnabled(bool)));
    connect(addition, SIGNAL(toggled(bool)),
            this, SLOT(additionStateChanged(bool)));
    connect(streaming, SIGNAL(toggled(bool)),
            streamingElement, SLOT(setEnabled(bool)));
    connect(clipboard, SIGNAL(toggled(bool)),
            clipboardElement, SLOT(setEnabled(bool)));
    connect(mouse, SIGNAL(toggled(bool)),
            mouseElement, SLOT(setEnabled(bool)));
    connect(filetransfer, SIGNAL(toggled(bool)),
            filetransferElement, SLOT(setEnabled(bool)));
    // dataChanged connections
    connect(address, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(networks, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(autoPort, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(port, SIGNAL(valueChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(tlsPortLabel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(tlsPort, SIGNAL(valueChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(usePassw, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(passw, SIGNAL(textEdited(QString)),
            this, SIGNAL(dataChanged()));
    connect(keymap, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(defaultPolicy, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(mainLabel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(displayLabel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(inputsLabel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(cursorLabel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(playbackLabel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(recordLabel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(smartcardLabel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(usbredirLabel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(main, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(display, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(inputs, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(cursor, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(playback, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(record, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(smartcard, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(usbredir, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(compress, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(compressImage, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(compressJpeg, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(compressZlib, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(compressPlayback, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(imageElement, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(jpegElement, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(zlibElement, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(playbackElement, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(addition, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(streaming, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(clipboard, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(mouse, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(filetransfer, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(streamingElement, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(clipboardElement, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(mouseElement, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(filetransferElement, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
}

/* public slots */
QDomDocument Spice_Graphics::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _listen, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("graphics");
    _devDesc.setAttribute("type", "spice");
    if ( tlsPortLabel->isChecked() )
        _devDesc.setAttribute("tlsPort", tlsPort->text());
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
    if ( policyElements->isVisibleTo(parentWidget()) ) {
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
    QString _address = address->itemData(
                address->currentIndex(), Qt::UserRole).toString();
    if ( !_address.isEmpty() && _address!="network" ) {
        _listen = doc.createElement("listen");
        _listen.setAttribute("type", "address");
        if ( _address!="custom" ) {
            _listen.setAttribute("address", _address);
            _devDesc.setAttribute("listen", _address);
        } else {
            _listen.setAttribute(
                        "address",
                        address->currentText());
            _devDesc.setAttribute(
                        "listen",
                        address->currentText());
        };
        _devDesc.appendChild(_listen);
    } else if ( _address=="network" && networks->count()>0 ) {
            _listen = doc.createElement("listen");
            _listen.setAttribute("type", "network");
            _listen.setAttribute("network", networks->currentText());
            _devDesc.appendChild(_listen);
    };
    if ( compress->isChecked() ) {
        if ( compressImage->isChecked() ) {
            QDomElement _image = doc.createElement("image");
            _image.setAttribute("compression", imageElement->currentText());
            _devDesc.appendChild(_image);
        };
        if ( compressJpeg->isChecked() ) {
            QDomElement _jpeg = doc.createElement("jpeg");
            _jpeg.setAttribute("compression", jpegElement->currentText());
            _devDesc.appendChild(_jpeg);
        };
        if ( compressZlib->isChecked() ) {
            QDomElement _zlib = doc.createElement("zlib");
            _zlib.setAttribute("compression", zlibElement->currentText());
            _devDesc.appendChild(_zlib);
        };
        if ( compressPlayback->isChecked() ) {
            QDomElement _playback = doc.createElement("playback");
            _playback.setAttribute("compression", playbackElement->currentText());
            _devDesc.appendChild(_playback);
        };
    };
    if ( addition->isChecked() ) {
        if ( streaming->isChecked() ) {
            QDomElement _streaming = doc.createElement("streaming");
            _streaming.setAttribute("mode", streamingElement->currentText());
            _devDesc.appendChild(_streaming);
        };
        if ( clipboard->isChecked() ) {
            QDomElement _clipboard = doc.createElement("clipboard");
            _clipboard.setAttribute("copypaste", clipboardElement->currentText());
            _devDesc.appendChild(_clipboard);
        };
        if ( mouse->isChecked() ) {
            QDomElement _mouse = doc.createElement("mouse");
            _mouse.setAttribute("mode", mouseElement->currentText());
            _devDesc.appendChild(_mouse);
        };
        if ( filetransfer->isChecked() ) {
            QDomElement _filetransfer = doc.createElement("filetransfer");
            _filetransfer.setAttribute("enable", filetransferElement->currentText());
            _devDesc.appendChild(_filetransfer);
        };
    };
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void Spice_Graphics::setDataDescription(QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    int idx;
    QDomElement _device, _listen,
            _streaming, _clipboard, _mouse, _filetransfer,
            _image, _jpeg, _zlib, _playback, _channel;
    doc.setContent(_xmlDesc);
    _device = doc.firstChildElement("device")
            .firstChildElement("graphics");
    autoPort->setChecked(
                _device.attribute("autoport")=="yes");
    if ( !autoPort->isChecked() )
        port->setValue(
                    _device.attribute("port").toInt());
    tlsPortLabel->setChecked( _device.hasAttribute("tlsPort") );
    if ( tlsPortLabel->isChecked() )
        tlsPort->setValue(
                    _device.attribute("tlsPort").toInt());
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
    QString _defaultPolicy = _device.attribute("defaultPolicy");
    idx = defaultPolicy->findText(
                _defaultPolicy, Qt::MatchContains);
    defaultPolicy->setCurrentIndex( (idx<0)? 0:idx );
    _channel = _device.firstChildElement("channel");
    while ( !_channel.isNull() ) {
        QComboBox *obj = NULL;
        QString _name, _mode;
        _name = _channel.attribute("name");
        _mode = _channel.attribute("mode");
        if ( _name=="main" ) {
            mainLabel->setChecked(true);
            obj = main;
        } else if ( _name=="display" ) {
            displayLabel->setChecked(true);
            obj = display;
        } else if ( _name=="inputs" ) {
            inputsLabel->setChecked(true);
            obj = inputs;
        } else if ( _name=="cursor" ) {
            cursorLabel->setChecked(true);
            obj = cursor;
        } else if ( _name=="playback" ) {
            playbackLabel->setChecked(true);
            obj = playback;
        } else if ( _name=="record" ) {
            recordLabel->setChecked(true);
            obj = record;
        } else if ( _name=="smartcard" ) {
            smartcardLabel->setChecked(true);
            obj = smartcard;
        } else if ( _name=="usbredir" ) {
            usbredirLabel->setChecked(true);
            obj = usbredir;
        };
        if ( NULL!=obj ) {
            idx = obj->findText(_mode, Qt::MatchContains);
            obj->setCurrentIndex( (idx<0)? 0:idx);
        };
        _channel = _channel.nextSiblingElement("channel");
    };
    _streaming = _device.firstChildElement("streaming");
    _clipboard = _device.firstChildElement("clipboard");
    _mouse = _device.firstChildElement("mouse");
    _filetransfer = _device.firstChildElement("filetransfer"),
    _image = _device.firstChildElement("image");
    _jpeg = _device.firstChildElement("jpeg");
    _zlib = _device.firstChildElement("zlib");
    _playback = _device.firstChildElement("playback ");
    streaming->setChecked( !_streaming.isNull() );
    clipboard->setChecked( !_clipboard.isNull() );
    mouse->setChecked( !_mouse.isNull() );
    filetransfer->setChecked( !_filetransfer.isNull() );
    compressImage->setChecked( !_image.isNull() );
    compressJpeg->setChecked( !_jpeg.isNull() );
    compressZlib->setChecked( !_zlib.isNull() );
    compressPlayback->setChecked( !_playback.isNull() );
    if ( !_streaming.isNull() ||
         !_clipboard.isNull() ||
         !_mouse.isNull()     ||
         !_filetransfer.isNull() ) {
        addition->setChecked(true);
        if ( !_streaming.isNull() ) {
            idx = streamingElement->findText(
                        _streaming.attribute("mode"));
            streamingElement->setCurrentIndex( (idx<0)? 0:idx );
        };
        if ( !_clipboard.isNull() ) {
            idx = clipboardElement->findText(
                        _clipboard.attribute("copypaste"));
            clipboardElement->setCurrentIndex( (idx<0)? 0:idx );
        };
        if ( !_mouse.isNull() ) {
            idx = mouseElement->findText(
                        _mouse.attribute("mode"));
            mouseElement->setCurrentIndex( (idx<0)? 0:idx );
        };
        if ( !_filetransfer.isNull() ) {
            idx = filetransferElement->findText(
                        _filetransfer.attribute("enable"));
            filetransferElement->setCurrentIndex( (idx<0)? 0:idx );
        }
    } else {
        addition->setChecked(false);
    };
    if ( !_image.isNull() ||
         !_jpeg.isNull()  ||
         !_zlib.isNull()  ||
         !_playback.isNull() ) {
        compress->setChecked(true);
        if ( !_image.isNull() ) {
            idx = imageElement->findText(
                        _image.attribute("compression"));
            imageElement->setCurrentIndex( (idx<0)? 0:idx );
        };
        if ( !_jpeg.isNull() ) {
            idx = jpegElement->findText(
                        _jpeg.attribute("compression"));
            jpegElement->setCurrentIndex( (idx<0)? 0:idx );
        };
        if ( !_zlib.isNull() ) {
            idx = zlibElement->findText(
                        _zlib.attribute("compression"));
            zlibElement->setCurrentIndex( (idx<0)? 0:idx );
        };
        if ( !_playback.isNull() ) {
            idx = playbackElement->findText(
                        _playback.attribute("compression"));
            playbackElement->setCurrentIndex( (idx<0)? 0:idx );
        }
    } else {
        compress->setChecked(false);
    };
}

/* private slots */
void Spice_Graphics::useAutoPort(bool state)
{
    port->setEnabled(!state);
    policyElementsSetRequired( !state && tlsPortLabel->isChecked()
                         && defaultPolicy->currentIndex()>0 );
}
void Spice_Graphics::useTLSPort(bool state)
{
    tlsPort->setEnabled(state);
    policyElementsSetRequired( state && !autoPort->isChecked()
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
void Spice_Graphics::policyElementsSetRequired(bool state)
{
    policyElements->setVisible(state);
}
void Spice_Graphics::defaultPolicyChanged(int i)
{
    if ( !policyElements->isVisible() ) {
        main->setCurrentIndex(i);
        display->setCurrentIndex(i);
        inputs->setCurrentIndex(i);
        cursor->setCurrentIndex(i);
        playback->setCurrentIndex(i);
        record->setCurrentIndex(i);
        smartcard->setCurrentIndex(i);
        usbredir->setCurrentIndex(i);
    };
    policyElementsSetRequired( tlsPortLabel->isChecked()
                         && !autoPort->isChecked()
                         && defaultPolicy->currentIndex()>0 );
}
void Spice_Graphics::compressStateChanged(bool state)
{
    compressElements->setVisible(state);
}
void Spice_Graphics::additionStateChanged(bool state)
{
    additionElements->setVisible(state);
}
void Spice_Graphics::readNetworkList()
{
    virNetworkPtr *networks = NULL;
    unsigned int flags = VIR_CONNECT_LIST_NETWORKS_ACTIVE |
                         VIR_CONNECT_LIST_NETWORKS_INACTIVE;
    int ret = virConnectListAllNetworks(currWorkConnection, &networks, flags);
    if ( ret<0 ) {
        sendConnErrors();
    } else {
        // therefore correctly to use for() command, because networks[0] can not exist.
        for (int i = 0; i < ret; i++) {
            nets.append( virNetworkGetName(networks[i]) );
            virNetworkFree(networks[i]);
        };
        free(networks);
    };
}

void Spice_Graphics::sendConnErrors()
{
    virtErrors = virConnGetLastError(currWorkConnection);
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
