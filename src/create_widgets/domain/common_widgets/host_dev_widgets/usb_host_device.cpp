#include "usb_host_device.h"

usb_hostHlpThread::usb_hostHlpThread(
        QObject         *parent,
        virConnectPtr   *connPtrPtr) :
    _VirtThread(parent, connPtrPtr)
{
    qRegisterMetaType<QStringList>("QStringList&");
}
void usb_hostHlpThread::run()
{
    if ( Q_NULLPTR==ptr_ConnPtr || Q_NULLPTR==*ptr_ConnPtr ) {
        emit ptrIsNull();
        return;
    };
    if ( virConnectRef(*ptr_ConnPtr)<0 ) {
        sendConnErrors();
        return;
    };
    QStringList       devices;
    virNodeDevicePtr *nodeDevices;
    unsigned int flags =
            VIR_CONNECT_LIST_NODE_DEVICES_CAP_USB_DEV;
    int ret = virConnectListAllNodeDevices(
                *ptr_ConnPtr, &nodeDevices, flags);
    if ( ret<0 ) {
        sendConnErrors();
    } else {
        // therefore correctly to use for() command,
        // because networks[0] can not exist.
        for (int i = 0; i < ret; i++) {
            devices.append( QString("%1\n")
                            // flags: extra flags; not used yet,
                            // so callers should always pass 0
                            .arg(virNodeDeviceGetXMLDesc(
                                     nodeDevices[i], 0)));
            virNodeDeviceFree(nodeDevices[i]);
        };
        if (nodeDevices) free(nodeDevices);
    };
    //int devs = virNodeNumOfDevices(ptr_ConnPtr, Q_NULLPTR, 0);
    if ( virConnectClose(*ptr_ConnPtr)<0 )
        sendConnErrors();
    emit result(devices);
}

USB_Host_Device::USB_Host_Device(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    _QWidget(parent, connPtrPtr)
{
    devList = new QListWidget(this);
    devList->setSelectionMode(QAbstractItemView::SingleSelection);
    devList->setSpacing(5);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(devList);
    setLayout(commonLayout);
    hlpThread = new usb_hostHlpThread(this, connPtrPtr);
    connect(hlpThread, SIGNAL(result(QStringList&)),
            this, SLOT(setAvailabledUSBDevices(QStringList&)));
    connect(hlpThread, SIGNAL(errorMsg(const QString&, const uint)),
            this, SIGNAL(errorMsg(const QString&)));
    connect(hlpThread, SIGNAL(finished()),
            this, SLOT(emitCompleteSignal()));
    hlpThread->start();
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
void USB_Host_Device::setAvailabledUSBDevices(QStringList &devices)
{
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
void USB_Host_Device::emitCompleteSignal()
{
    if ( sender()==hlpThread ) {
        setEnabled(true);
        emit complete();
    }
}
