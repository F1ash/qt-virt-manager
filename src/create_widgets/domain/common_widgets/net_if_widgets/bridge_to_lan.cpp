#include "bridge_to_lan.h"

Bridge_to_LAN::Bridge_to_LAN(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    bridgeLabel = new QLabel("Bridge:", this);
    bridge = new QLineEdit(this);
    targetLabel = new QLabel("Target Device name:", this);
    target = new QLineEdit(this);
    baseLayout = new QGridLayout();
    baseLayout->addWidget(bridgeLabel, 0, 0);
    baseLayout->addWidget(bridge, 0, 1);
    baseLayout->addWidget(targetLabel, 1, 0);
    baseLayout->addWidget(target, 1, 1);
    mac = new MAC_Address(this);
    virtPort = new VirtualPort(this);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(mac);
    commonLayout->addWidget(virtPort);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    virtPort->type->setCurrentIndex( virtPort->type->findText("Open vSwitch") );
}

/* public slots */
QDomDocument Bridge_to_LAN::getDataDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source,_target, _mac, _virtualport,
            _parameters, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("interface");
    _source = doc.createElement("source");
    _source.setAttribute("bridge", bridge->text());
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
    _devDesc.setAttribute("type", "bridge");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
