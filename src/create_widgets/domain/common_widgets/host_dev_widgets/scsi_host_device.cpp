#include "scsi_host_device.h"

SCSI_Host_Device::SCSI_Host_Device(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    devList = new QListWidget(this);
    devList->setSelectionMode(QAbstractItemView::SingleSelection);
    devList->setSpacing(5);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(devList);
    setLayout(commonLayout);
    setAvailabledSCSIDevices();
}

/* public slots */
QDomDocument SCSI_Host_Device::getDataDocument() const
{
    QDomDocument doc;
    if ( devList->selectedItems().count()>0 ) {
        QString _Addr, _host, _bus, _target, _unit;
        _Addr = devList->selectedItems().first()->data(Qt::UserRole).toString();
        QStringList _split = _Addr.split(":");
        if ( _split.count()>3 ) {
            _host = _split.at(0);
            _bus = _split.at(1);
            _target = _split.at(2);
            _unit = _split.at(3);
        };
        QDomElement _source, _adapter, _address, _device, _devDesc;
        _device = doc.createElement("device");
        _devDesc = doc.createElement("hostdev");
        _source = doc.createElement("source");
        _adapter = doc.createElement("adapter");
        _adapter.setAttribute(
                    "name", QString("scsi_host%1").arg(_host));
        _source.appendChild(_adapter);

        _address = doc.createElement("address");
        _address.setAttribute("bus", _bus);
        _address.setAttribute("target", _target);
        _address.setAttribute("unit", _unit);
        _source.appendChild(_address);

        _devDesc.appendChild(_source);
        _device.appendChild(_devDesc);
        _devDesc.setAttribute("type", "scsi");
        _devDesc.setAttribute("mode", "subsystem");
        // WARNING: unknown rule to set
        //_devDesc.setAttribute("managed", "???");
        doc.appendChild(_device);
    };
    return doc;
}

/* private slots */
void SCSI_Host_Device::setAvailabledSCSIDevices()
{
    int i = 0;
    QStringList      devices;
    virNodeDevice  **nodeDevices = NULL;
    if ( currWorkConnect!=NULL ) {
        unsigned int flags =
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_SCSI ;
        int ret = virConnectListAllNodeDevices(currWorkConnect, &nodeDevices, flags);
        if ( ret<0 ) {
            sendConnErrors();
        } else {
            while ( nodeDevices[i] != NULL ) {
                devices.append( QString("%1\n")
                                // flags: extra flags; not used yet,
                                // so callers should always pass 0
                                .arg(virNodeDeviceGetXMLDesc(nodeDevices[i], 0)));
                virNodeDeviceFree(nodeDevices[i]);
                i++;
            };
        };
        free(nodeDevices);
    };
    //int devs = virNodeNumOfDevices(currWorkConnect, NULL, 0);
    //qDebug()<<"Devices("<<devs<<i<<"):\n"<<devices.join("\n");
    // set unique device description to devList
    foreach (QString _dev, devices) {
        //qDebug()<<_dev;
        QString devName, devIdentity, host, bus, target, lun;
        QDomElement name, path, type;
        QDomDocument doc;
        doc.setContent(_dev);
        name = doc.firstChildElement("device").
                firstChildElement("name");
        path = doc.firstChildElement("device").
                firstChildElement("path");
        type = doc.firstChildElement("device").
                firstChildElement("capability").
                firstChildElement("type");
        host = doc.firstChildElement("device").
                firstChildElement("capability").
                firstChildElement("host").
                firstChild().toText().data();
        bus = doc.firstChildElement("device").
                firstChildElement("capability").
                firstChildElement("bus").
                firstChild().toText().data();
        target = doc.firstChildElement("device").
                firstChildElement("capability").
                firstChildElement("target").
                firstChild().toText().data();
        lun = doc.firstChildElement("device").
                firstChildElement("capability").
                firstChildElement("lun").
                firstChild().toText().data();
        devIdentity.append(host);
        devIdentity.append(":");
        devIdentity.append(bus);
        devIdentity.append(":");
        devIdentity.append(target);
        devIdentity.append(":");
        devIdentity.append(lun);
        // devName format: <name_(type)_identity_path>
        devName.append(name.firstChild().toText().data());
        devName.append("\t(");
        devName.append(type.firstChild().toText().data().toUpper());
        devName.append(")\n");
        devName.append(QString("Host:Bus:Target:Lun (%1)").arg(devIdentity));
        devName.append("\n");
        devName.append(path.firstChild().toText().data());
        if ( devList->findItems(devName,
                                Qt::MatchExactly |
                                Qt::MatchCaseSensitive)
             .isEmpty() ) {
            devList->insertItem(0, devName);
            devList->item(0)->setData(Qt::UserRole, devIdentity);
        };
    };
    devList->setCurrentRow(0);
}

void SCSI_Host_Device::sendConnErrors()
{
    virtErrors = virConnGetLastError(currWorkConnect);
    if ( virtErrors!=NULL && virtErrors->code>0 ) {
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                       .arg(QString().fromUtf8(virtErrors->message)) );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void SCSI_Host_Device::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL && virtErrors->code>0 )
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                       .arg(QString().fromUtf8(virtErrors->message)) );
    virResetLastError();
}
