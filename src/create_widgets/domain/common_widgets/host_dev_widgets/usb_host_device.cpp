#include "usb_host_device.h"

USB_Host_Device::USB_Host_Device(
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
    setAvailabledUSBDevices();
}

/* public slots */
QDomDocument USB_Host_Device::getDataDocument() const
{
    QDomDocument doc;
    QString _Id, vendorId, productId;
    if ( devList->selectedItems().count()>0 ) {
        _Id = devList->selectedItems().first()->data(Qt::UserRole).toString();
        vendorId.append(_Id.split(":").first());
        productId.append(_Id.split(":").last());
        QDomElement _source, _product, _vendor, _device, _devDesc;
        _device = doc.createElement("device");
        _devDesc = doc.createElement("hostdev");
        _source = doc.createElement("source");
        _vendor = doc.createElement("vendor");
        _vendor.setAttribute("id", vendorId);
        _product = doc.createElement("product");
        _product.setAttribute("id", productId);
        _source.appendChild(_vendor);
        _source.appendChild(_product);
        _devDesc.appendChild(_source);
        _devDesc.setAttribute("type", "usb");
        _devDesc.setAttribute("mode", "subsystem");
        // WARNING: see for virt-manager application
        _devDesc.setAttribute("managed", "yes");
        _device.appendChild(_devDesc);
        doc.appendChild(_device);
    };
    return doc;
}

/* private slots */
void USB_Host_Device::setAvailabledUSBDevices()
{
    int i = 0;
    QStringList      devices;
    virNodeDevice  **nodeDevices = NULL;
    if ( currWorkConnection!=NULL ) {
        unsigned int flags =
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_USB_DEV;
        int ret = virConnectListAllNodeDevices(currWorkConnection, &nodeDevices, flags);
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
    //int devs = virNodeNumOfDevices(currWorkConnection, NULL, 0);
    //qDebug()<<"Devices("<<devs<<i<<"):\n"<<devices.join("\n");
    // set unique device description to devList
    foreach (QString _dev, devices) {
        //qDebug()<<_dev;
        QString devName, devIdentity;
        QDomElement capability, product, vendor;
        QDomDocument doc = QDomDocument();
        doc.setContent(_dev);
        // filter out **** Host Controllers
        if ( !doc.
             firstChildElement("device").
             firstChildElement("parent").
             firstChild().toText().data().
             startsWith("usb") ) continue;
        //
        capability = doc.firstChildElement("device").
                firstChildElement("capability");
        product = capability.firstChildElement("product");
        vendor = capability.firstChildElement("vendor");
        // devIdentity format: <vendor:product>
        devIdentity.append(vendor.attribute("id"));
        devIdentity.append(":");
        devIdentity.append(product.attribute("id"));
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
    devList->setCurrentRow(0);
}

void USB_Host_Device::sendConnErrors()
{
    virtErrors = virConnGetLastError(currWorkConnection);
    if ( virtErrors!=NULL && virtErrors->code>0 ) {
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                       .arg(QString().fromUtf8(virtErrors->message)) );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void USB_Host_Device::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL && virtErrors->code>0 )
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                       .arg(QString().fromUtf8(virtErrors->message)) );
    virResetLastError();
}
