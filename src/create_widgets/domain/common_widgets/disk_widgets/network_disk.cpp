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
    startupPolicy->setVisible(false);
    connect(protocol, SIGNAL(currentIndexChanged(int)),
            this, SLOT(protocolTypeChanged(int)));
    // dataChanged connections
    connect(protocol, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(protocol, SIGNAL(editTextChanged(QString)),
            this, SLOT(stateChanged()));
    connect(sourceName, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument Network_Disk::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _source, _target, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("disk");

    _source = doc.createElement("source");
    _source.setAttribute("protocol", protocol->currentText());
    if ( !sourceName->text().isEmpty() )
        _source.setAttribute("name", sourceName->text());
    //if ( startupPolicy->isUsed() )
    //    _source.setAttribute("startupPolicy",
    //                         startupPolicy->getStartupPolicy());
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
    };

    _devDesc.setAttribute("type", "network");
    _devDesc.setAttribute("device", devType->getDeviceType());
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void Network_Disk::setDataDescription(QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _source, _host,
            _target, _readOnly, _driver;
    _device = doc.firstChildElement("device")
            .firstChildElement("disk");
    _source = _device.firstChildElement("source");
    _target = _device.firstChildElement("target");
    _readOnly = _device.firstChildElement("readonly");
    _driver = _device.firstChildElement("driver");
    QString _attr;
    int idx;
    _attr = _device.attribute("device");
    devType->setDeviceType(_attr);
    _attr = _source.attribute("protocol");
    idx = protocol->findText(
                _attr, Qt::MatchContains);
    if ( idx<0 ) {
        protocol->setCurrentIndex(protocol->count()-1);
        protocol->setEditText(_attr);
    } else
        protocol->setCurrentIndex(idx);
    _attr = _source.attribute("name");
    sourceName->setText(_attr);
    _attr = _target.attribute("dev");
    target->devName->setText(_attr);
    _host = _source.firstChildElement("host");
    hosts->checkHosts( !_host.isNull() );
    hosts->clearHostList();
    _attr.clear();
    while ( !_host.isNull() ) {
        _attr.append(_host.attribute("name"));
        _attr.append(":");
        _attr.append(_host.attribute("port"));
        hosts->setHostItem(_attr);
        _host = _host.nextSiblingElement("host");
    };
    _attr = _target.attribute("bus");
    idx = target->bus->findText(_attr, Qt::MatchContains);
    target->bus->setCurrentIndex( (idx<0)? 0:idx );
    if ( _target.hasAttribute("tray") ) {
        target->trayLabel->setChecked(true);
        _attr = _target.attribute("tray");
        idx = target->tray->findText(_attr, Qt::MatchContains);
        target->tray->setCurrentIndex( (idx<0)? 0:idx );
    } else
        target->trayLabel->setChecked(false);
    if ( _target.hasAttribute("removable") ) {
        target->removableLabel->setChecked(true);
        _attr = _target.attribute("removable");
        idx = target->removable->findText(_attr, Qt::MatchContains);
        target->removable->setCurrentIndex( (idx<0)? 0:idx );
    } else
        target->removableLabel->setChecked(false);
    readOnly->readOnly->setChecked( !_readOnly.isNull() );
}

/* private slots */
void Network_Disk::protocolTypeChanged(int i)
{
    protocol->setEditable( i==protocol->count()-1 );
    protocol->clearEditText();
}
