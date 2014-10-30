#include "direct_attachment.h"

DirectAttachment::DirectAttachment(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    netSourceLabel = new QLabel("Network source:", this);
    sourceModeLabel = new QLabel("Source mode:", this);
    netSource = new QComboBox(this);
    sourceMode = new QComboBox(this);
    sourceMode->addItem("VEPA");
    sourceMode->addItem("Bridge");
    sourceMode->addItem("Private");
    sourceMode->addItem("PassThrough");
    baseLayout = new QGridLayout();
    baseLayout->addWidget(netSourceLabel, 0, 0);
    baseLayout->addWidget(netSource, 0, 1);
    baseLayout->addWidget(sourceModeLabel, 1, 0);
    baseLayout->addWidget(sourceMode, 1, 1);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    mac = new MAC_Address(this);
    nicModel = new NIC_Model(this);
    virtPort = new VirtualPort(this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(mac);
    commonLayout->addWidget(nicModel);
    commonLayout->addWidget(virtPort);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    setAvailableSources();
    virtPort->type->setCurrentIndex( virtPort->type->findText("802.1Qbh") );
}

/* public slots */
QDomDocument DirectAttachment::getDataDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _mac, _virtualport, _model,
            _parameters, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("interface");
    _source = doc.createElement("source");
    QString _dev = netSource->itemData(netSource->currentIndex(), Qt::UserRole).toString();
    if ( !_dev.isEmpty() ) _source.setAttribute("dev", _dev);
    _source.setAttribute("mode", sourceMode->currentText().toLower());
    _devDesc.appendChild(_source);

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

    if ( !mac->getMACAddress().isEmpty() ) {
        _mac = doc.createElement("mac");
        _mac.setAttribute("address", mac->getMACAddress());
        _devDesc.appendChild(_mac);
    };

    if ( !nicModel->getDevModel().isEmpty() ) {
        _model = doc.createElement("model");
        _model.setAttribute("type", nicModel->getDevModel());
        _devDesc.appendChild(_model);
    };
    _devDesc.setAttribute("type", "direct");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}

/* private slots */
void DirectAttachment::setAvailableSources()
{
    virNodeDevice  **nodeDevices = NULL;
    if ( currWorkConnect!=NULL ) {
        unsigned int flags =
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_NET;
        int ret = virConnectListAllNodeDevices(currWorkConnect, &nodeDevices, flags);
        if ( ret<0 ) {
            netSource->insertItem(0, "NetSource detect failed", "");
        } else {
            int i = 0;
            while ( nodeDevices[i] != NULL ) {
                QDomDocument doc;
                QString _dev, _interface;
                // flags: extra flags; not used yet,
                // so callers should always pass 0
                _dev = QString(virNodeDeviceGetXMLDesc(nodeDevices[i], 0));
                //qDebug()<<_dev;
                doc.setContent(_dev);
                _interface = doc.
                        firstChildElement("device").
                        firstChildElement("capability").
                        firstChildElement("interface").
                        firstChild().toText().data();
                if ( _interface!="lo" )
                    netSource->insertItem(
                            0,
                            QString("Host Device '%1' : macvtap")
                            .arg(_interface),
                            _interface);
                virNodeDeviceFree(nodeDevices[i]);
                i++;
            };
        };
        free(nodeDevices);
    };
}
