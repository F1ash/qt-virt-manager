#include "generic_ethernet.h"

Generic_Ethernet::Generic_Ethernet(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    targetLabel = new QLabel("Target Device name:", this);
    scriptLabel = new QLabel("Script path:", this);
    target = new QLineEdit(this);
    target->setPlaceholderText("vnetN");
    script = new QLineEdit(this);
    script->setPlaceholderText("/etc/qemu-ifup");
    commonLayout = new QGridLayout();
    commonLayout->addWidget(targetLabel, 0, 0);
    commonLayout->addWidget(target, 0, 1);
    commonLayout->addWidget(scriptLabel, 1, 0);
    commonLayout->addWidget(script, 1, 1);
    setLayout(commonLayout);
}

/* public slots */
QDomDocument Generic_Ethernet::getDataDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _target, _script, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("interface");
    QString _dev, _scr;
    _dev = target->text();
    _scr = script->text();
    if ( !_dev.isEmpty() ) {
        _target = doc.createElement("target");
        _target.setAttribute("dev", _dev);
        _devDesc.appendChild(_target);
    };
    if ( !_scr.isEmpty() ) {
        _script = doc.createElement("script");
        _script.setAttribute("path", _scr);
        _devDesc.appendChild(_script);
    };
    _devDesc.setAttribute("type", "ethernet");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
