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
/*
QDomNodeList SCSI_Host_Device::getNodeList() const
{
    QDomDocument doc;
    QDomElement _source, _address;
    _source = doc.createElement("source");
    _address = doc.createElement("address");
    _source.appendChild(_address);
    doc.appendChild(_source);
    return doc.childNodes();
}
QString SCSI_Host_Device::getDevType() const
{
    return QString("scsi");
}
QString SCSI_Host_Device::getDevMode() const
{
    return QString("subsystem");
}
*/

/* private slots */
void SCSI_Host_Device::setAvailabledSCSIDevices()
{
    int i = 0;
    QStringList      devices;
    virNodeDevice  **nodeDevices = NULL;
    if ( currWorkConnect!=NULL ) {
        unsigned int flags =
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_SCSI;
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
        //qDebug()<<_dev;
        QString devName, devIdentity;
        QDomElement capability, domain, bus, slot, function, vendor, product;
        QDomDocument doc = QDomDocument();
        doc.setContent(_dev);
        capability = doc.firstChildElement("device").
                firstChildElement("capability");
        domain = capability.firstChildElement("domain");
        bus = capability.firstChildElement("bus");
        slot = capability.firstChildElement("slot");
        function = capability.firstChildElement("function");
        vendor = capability.firstChildElement("vendor");
        product = capability.firstChildElement("product");
        // devIdentity format: <domain:bus:slot:function>
        devIdentity.append(domain.firstChild().toText().data());
        devIdentity.append(":");
        devIdentity.append(bus.firstChild().toText().data());
        devIdentity.append(":");
        devIdentity.append(slot.firstChild().toText().data());
        devIdentity.append(":");
        devIdentity.append(function.firstChild().toText().data());
        // devName format: <vendor_product>
        devName.append(vendor.firstChild().toText().data());
        devName.append("\n");
        devName.append(product.firstChild().toText().data());
        if ( devList->findItems(devName,
                                Qt::MatchExactly |
                                Qt::MatchCaseSensitive)
             .isEmpty() ) {
            devList->insertItem(0, devName);
            devList->item(0)->setData(Qt::UserRole, devIdentity);
        };
    };
    devList->insertItem(0, "WARNING:\nDon't implemented");
    devList->item(0)->setIcon(QIcon::fromTheme("dialog-warning").pixmap(32));
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
