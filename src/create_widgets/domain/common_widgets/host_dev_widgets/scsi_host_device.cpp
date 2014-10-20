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
    QString _Addr, domain, bus, slot, function;
    if ( devList->selectedItems().count()>0 ) {
        _Addr = devList->selectedItems().first()->data(Qt::UserRole).toString();
        QStringList _AddrList = _Addr.split(":");
        domain.append( (_AddrList.size()>0)? _AddrList.at(0) : "0" );
        bus.append( (_AddrList.size()>1)? _AddrList.at(1) : "0" );
        slot.append( (_AddrList.size()>2)? _AddrList.at(2) : "0" );
        function.append( (_AddrList.size()>3)? _AddrList.at(3) : "0" );
        QDomElement _source, _adapter, _address, _device, _devDesc;
        _device = doc.createElement("device");
        _devDesc = doc.createElement("hostdev");
        _source = doc.createElement("source");
        _adapter = doc.createElement("adapter");
        _adapter.setAttribute(
                    "name",
                    devList->selectedItems().first()->text()
                    .split("\n").first());
        _source.appendChild(_adapter);
        _devDesc.appendChild(_source);

        _address = doc.createElement("address");
        _address.setAttribute("domain", QString("0x%1").arg(domain));
        _address.setAttribute("bus", QString("0x%1").arg(bus));
        _address.setAttribute("slot", QString("0x%1").arg(slot));
        _address.setAttribute("function", function);
        _devDesc.appendChild(_address);

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
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_SCSI_HOST ;
                //VIR_CONNECT_LIST_NODE_DEVICES_CAP_SCSI |
                //VIR_CONNECT_LIST_NODE_DEVICES_CAP_SCSI_GENERIC |
                //VIR_CONNECT_LIST_NODE_DEVICES_CAP_SCSI_TARGET;
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
    /*
     * WARNING: For SCSI devices, user is responsible to make sure
     * the device is not used by host.
     * SCSI devices are described by both the adapter and address elements.
     */
    foreach (QString _dev, devices) {
        qDebug()<<_dev;
        QString devName, devIdentity;
        QDomElement name, path, parent;
        QDomDocument doc;
        doc.setContent(_dev);
        name = doc.firstChildElement("device").
                firstChildElement("name");
        path = doc.firstChildElement("device").
                firstChildElement("path");
        parent = doc.firstChildElement("device").
                firstChildElement("parent");
        QStringList _split = parent.firstChild().toText().data().split("_");
        // devIdentity format: <PCIAddress>
        devIdentity.append(_split.at(1));
        devIdentity.append(":");
        devIdentity.append(_split.at(2));
        devIdentity.append(":");
        devIdentity.append(_split.at(3));
        devIdentity.append(":");
        devIdentity.append(_split.at(4));
        // devName format: <name_path>
        devName.append(name.firstChild().toText().data());
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
    //devList->insertItem(0, "WARNING:\nDon't implemented");
    //devList->item(0)->setIcon(QIcon::fromTheme("dialog-warning").pixmap(32));
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
