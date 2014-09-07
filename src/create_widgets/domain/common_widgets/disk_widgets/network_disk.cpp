#include "network_disk.h"

#define PROTOCOLS QStringList()\
    <<"nbd"<<"iscsi"<<"rbd"<<"sheepdog"<<"gluster"

Network_Disk::Network_Disk(
        QWidget *parent,
        virConnectPtr conn) :
    _Disk(parent, conn)
{
    protocolLabel = new QLabel("Protocol:", this);
    protocol = new QComboBox(this);
    protocol->addItems(PROTOCOLS);
    protocol->insertSeparator(protocol->count());
    protocol->insertItem(protocol->count(), "Set manually");
    sourceNameLabel = new QLabel("Source name:", this);
    sourceName = new QLineEdit(this);
    sourceName->setPlaceholderText("Source name or URL path");

    baseLayout->addWidget(protocolLabel, 0, 0);
    baseLayout->addWidget(protocol, 0, 1);
    baseLayout->addWidget(sourceNameLabel, 1, 0);
    baseLayout->addWidget(sourceName, 1, 1);

    hosts->setVisible(true);
    connect(protocol, SIGNAL(currentIndexChanged(int)),
            this, SLOT(protocolTypeChanged(int)));
}

/* public slots */
QDomDocument Network_Disk::getDevDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _target, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("disk");

    _source = doc.createElement("source");
    _source.setAttribute("protocol", protocol->currentText());
    if ( !sourceName->text().isEmpty() )
        _source.setAttribute("name", sourceName->text());
    if ( startupPolicy->isUsed() )
        _source.setAttribute("startupPolicy",
                             startupPolicy->getStartupPolicy());
    if ( hosts->isUsed() ) {
        foreach (QString _h, hosts->getHostsList()) {
            if ( !_h.isEmpty() ) {
                QString _name, _port;
                QStringList _split;
                _split = _h.split(":");
                _name = _split.first();
                _port = _split.last();
                QDomElement _host = doc.createElement("host");
                _host.setAttribute("name", _name);
                _host.setAttribute("port", _port);
                _source.appendChild(_host);
            };
        };
    };
    _devDesc.appendChild(_source);

    _target = doc.createElement("target");
    TargetAttrs l = target->getTargetAttrs();
    foreach (QString key, l.keys()) {
        if ( !key.isEmpty() )
        _target.setAttribute(key, l.value(key));
    };
    _devDesc.appendChild(_target);

    if ( readOnly->state() ) {
        QDomElement _readOnly = doc.createElement("readonly");
        _devDesc.appendChild(_readOnly);
    }

    _devDesc.setAttribute("type", "network");
    _devDesc.setAttribute("device", devType->getDeviceType());
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}

/* private slots */
void Network_Disk::protocolTypeChanged(int i)
{
    protocol->setEditable( i==protocol->count()-1 );
    protocol->clearEditText();
}
