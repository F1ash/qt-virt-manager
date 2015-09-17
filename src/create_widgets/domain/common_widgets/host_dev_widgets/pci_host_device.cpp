#include "pci_host_device.h"

pci_hostHlpThread::pci_hostHlpThread(QObject *parent, virConnectPtr* connPtrPtr) :
    _VirtThread(parent, connPtrPtr)
{
    qRegisterMetaType<QStringList>("QStringList&");
}
void pci_hostHlpThread::run()
{
    if ( NULL==ptr_ConnPtr ) return;
    if ( virConnectRef(*ptr_ConnPtr)<0 ) {
        sendConnErrors();
        return;
    };
    int i = 0;
    QStringList      devices;
    virNodeDevice  **nodeDevices = NULL;
    unsigned int flags =
            VIR_CONNECT_LIST_NODE_DEVICES_CAP_PCI_DEV;
    int ret = virConnectListAllNodeDevices(*ptr_ConnPtr, &nodeDevices, flags);
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
    //int devs = virNodeNumOfDevices(ptr_ConnPtr, NULL, 0);
    if ( virConnectClose(*ptr_ConnPtr)<0 )
        sendConnErrors();
    emit result(devices);
}

PCI_Host_Device::PCI_Host_Device(
        QWidget *parent, virConnectPtr *connPtrPtr) :
    _QWidget(parent, connPtrPtr)
{
    devList = new QListWidget(this);
    devList->setSelectionMode(QAbstractItemView::SingleSelection);
    devList->setSpacing(5);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(devList);
    setLayout(commonLayout);
    hlpThread = new pci_hostHlpThread(this, connPtrPtr);
    connect(hlpThread, SIGNAL(result(QStringList&)),
            this, SLOT(setAvailabledPCIDevices(QStringList&)));
    connect(hlpThread, SIGNAL(errorMsg(QString&,uint)),
            this, SIGNAL(errorMsg(QString&)));
    hlpThread->start();
}

/* public slots */
QDomDocument PCI_Host_Device::getDataDocument() const
{
    QDomDocument doc;
    QString _Addr, domain, bus, slot, function, _hex;
    if ( devList->selectedItems().count()>0 ) {
        _Addr = devList->selectedItems().first()->data(Qt::UserRole).toString();
        QStringList _AddrList = _Addr.split(":");
        // if some from parameters is not set, then don't set whole element
        QDomElement _source, _address, _device, _devDesc;
        _device = doc.createElement("device");
        _devDesc = doc.createElement("hostdev");
        if ( _AddrList.size()>3 ) {
            domain.append( (_AddrList.size()>0)? _AddrList.at(0) : "0" );
            bus.append( (_AddrList.size()>1)? _AddrList.at(1) : "0" );
            slot.append( (_AddrList.size()>2)? _AddrList.at(2) : "0" );
            function.append( (_AddrList.size()>3)? _AddrList.at(3) : "0" );
            _source = doc.createElement("source");
            _address = doc.createElement("address");

            _hex = QString::number(domain.toInt(), 16);
            _address.setAttribute("domain", QString("0x%1").arg(_hex));
            _hex = QString::number(bus.toInt(), 16);
            _address.setAttribute("bus", QString("0x%1").arg(_hex));
            _hex = QString::number(slot.toInt(), 16);
            _address.setAttribute("slot", QString("0x%1").arg(_hex));
            _address.setAttribute("function", function);

            _source.appendChild(_address);
            _devDesc.appendChild(_source);
        };
        _device.appendChild(_devDesc);
        _devDesc.setAttribute("type", "pci");
        _devDesc.setAttribute("mode", "subsystem");
        // WARNING: unknown rule to set
        //_devDesc.setAttribute("managed", "???");
        doc.appendChild(_device);
    };
    return doc;
}

/* private slots */
void PCI_Host_Device::setAvailabledPCIDevices(QStringList &devices)
{
    //qDebug()<<"Devices("<<devs<<i<<"):\n"<<devices.join("\n");
    // set unique device description to devList
    // WARNING: PCI devices can only be described by their address.
    foreach (QString _dev, devices) {
        //qDebug()<<_dev;
        QString devName, devIdentity;
        QDomElement capability, domain, bus, slot, function, vendor, product;
        QDomDocument doc;
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
    devList->setCurrentRow(0);
}
