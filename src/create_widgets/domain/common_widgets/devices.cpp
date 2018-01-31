#include "devices.h"

/*
 * http://libvirt.org/formatdomain.html
 *
    <!- -------------------------- ->
    Devices
        Emulator
        Hard drives, floppy disks, CDROMs
        Filesystems
        Device Addresses
        Controllers
        Device leases
        Host device assignment
            USB / PCI / SCSI devices
            Block / character devices
        Redirected devices
        Smartcard devices
        Network interfaces
            Virtual network
            Bridge to LAN
            Userspace SLIRP stack
            Generic ethernet connection
            Direct attachment to physical interface
            PCI Passthrough
            Multicast tunnel
            TCP tunnel
            Setting the NIC model
            Setting NIC driver-specific options
            Overriding the target element
            Specifying boot order
            Interface ROM BIOS configuration
            Quality of service
            Setting VLAN tag (on supported network types only)
            Modifying virtual link state
        Input devices
        Hub devices
        Graphical framebuffers
        Video devices
        Consoles, serial, parallel & channel devices
            Guest interface
                Parallel port
                Serial port
                Console
                Channel
            Host interface
                Domain logfile
                Device logfile
                Virtual console
                Null device
                Pseudo TTY
                Host device proxy
                Named pipe
                TCP client/server
                UDP network console
                UNIX domain socket client/server
                Spice channel
        Sound devices
        Watchdog device
        Memory balloon device
        Random number generator device
        TPM device
        NVRAM device
        panic device
    <!- -------------------------- ->

*/

Devices::Devices(
        QWidget *parent, virConnectPtr *connPtrPtr, QString _xmlDesc) :
    _QWidget(parent, connPtrPtr), xmlDesc(_xmlDesc)
{
    setObjectName("Devices");
    usedDeviceList = new QListWidget(this);
    usedDeviceList->setSelectionMode(QAbstractItemView::SingleSelection);
    usedDeviceList->setContextMenuPolicy(Qt::CustomContextMenu);
    usedDeviceList->setSortingEnabled(false);
    connect(usedDeviceList, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showContextMenu(const QPoint&)));
    addNewDevice = new QPushButton(QIcon::fromTheme("list-add"), "", this);
    addNewDevice->setToolTip(tr("Add Device"));
    connect(addNewDevice, SIGNAL(clicked()),
            this, SLOT(showDeviceStack()));
    delSelDevice = new QPushButton(
                QIcon::fromTheme("list-remove"), "", this);
    delSelDevice->setToolTip(tr("Del Device"));
    connect(delSelDevice, SIGNAL(clicked()),
            this, SLOT(delDevice()));

    buttonlayout = new QHBoxLayout(this);
    buttonlayout->addWidget(addNewDevice);
    buttonlayout->addWidget(delSelDevice);
    buttons = new QWidget(this);
    buttons->setLayout(buttonlayout);

    listLayout = new QVBoxLayout(this);
    listLayout->addWidget(usedDeviceList);
    listLayout->addWidget(buttons);
    listWidget = new QWidget(this);
    listWidget->setLayout(listLayout);

    infoWidget = new DeviceData(this, ptr_ConnPtr);
    baseLayout = new QHBoxLayout(this);
    baseLayout->addWidget(listWidget, 3);
    baseLayout->addWidget(infoWidget, 8);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);

    deviceStack = new DeviceStack(this, ptr_ConnPtr);
    stackWdg = new QStackedWidget(this);
    stackWdg->addWidget(baseWdg);
    stackWdg->addWidget(deviceStack);

    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(stackWdg);
    setLayout(commonLayout);

    detectAttachedDevicesFromXMLDesc();
    connect(usedDeviceList, SIGNAL(itemSelectionChanged()),
            this, SLOT(showDevice()));
    connect(infoWidget, SIGNAL(saveDeviceXMLDesc(const int, const QString&)),
            this, SLOT(saveDeviceXMLDescription(const int, const QString&)));
    connect(infoWidget, SIGNAL(errorMsg(const QString&)),
            this, SIGNAL(errorMsg(const QString&)));
    connect(infoWidget, SIGNAL(dataProcessed(bool)),
            listWidget, SLOT(setEnabled(bool)));
    connect(deviceStack, SIGNAL(finished(int)),
            this, SLOT(addDevice()));
    connect(deviceStack, SIGNAL(errorMsg(const QString&)),
            this, SIGNAL(errorMsg(const QString&)));
}

/* public slots */
QDomDocument Devices::getDataDocument() const
{
    /*
     * parse usedDeviceList
     */
    //qDebug()<<"Device result";
    QDomDocument doc;
    infoWidget->closeDataEdit();
    QDomElement devices = doc.createElement("devices");
    for (int i=0; i<usedDeviceList->count(); i++) {
        QListWidgetItem *item = usedDeviceList->item(i);
        if ( nullptr==item ) continue;
        QDomDocument _doc;
        _doc.setContent(item->data(Qt::UserRole).toString());
        QDomNodeList list = _doc.firstChildElement("device").childNodes();
        int j = 0;
        int count = list.length();
        for (int k=0; k<count;k++) {
            //qDebug()<<list.item(j).nodeName()<<k;
            if (!list.item(j).isNull()) {
                devices.appendChild(list.item(j));
            } else ++j;
        };
    };
    doc.appendChild(devices);
    //qDebug()<<doc.toString()<<"Device result";
    return doc;
}
void Devices::setEmulator(const QString &_emulator)
{
    QList<QListWidgetItem*> _family =
            usedDeviceList->findItems(
                "emulator", Qt::MatchContains);
    QDomDocument doc;
    QDomElement _dev, _emul;
    _dev = doc.createElement("device");
    _emul = doc.createElement("emulator");
    QDomText data = doc.createTextNode(_emulator);
    _emul.appendChild(data);
    _dev.appendChild(_emul);
    doc.appendChild(_dev);
    if ( !_family.isEmpty() ) {
        QListWidgetItem *item =
                usedDeviceList->takeItem(
                    usedDeviceList->row(_family.at(0)));
        if ( nullptr!=item ) {
            delete item;
            item = nullptr;
        };
    };
    // drop the infinite loop
    addDeviceToUsedDevList(doc, false);
}
void Devices::initBootDevices()
{
    QDomDocument _doc = getDataDocument();
    emit devicesChanged(_doc);
}

/* private slots */
void Devices::showDeviceStack()
{
    stackWdg->setCurrentWidget(deviceStack);
}
void Devices::addDevice()
{
    if ( deviceStack->result()==QDialog::Accepted ) {
        QDomDocument doc = deviceStack->getResult();
        //qDebug()<<doc.toString();
        addDeviceToUsedDevList(doc);
    };
    deviceStack->clearDevice();
    stackWdg->setCurrentWidget(baseWdg);
}
void Devices::addDeviceToUsedDevList(QDomDocument &doc, const bool flag)
{
    QDomElement _el = doc.firstChildElement("device");
    if ( _el.isNull() ) return;
    QDomNodeList list = _el.childNodes();
    if ( list.isEmpty() ) return;
    QString device, desc, name;
    device = list.item(0).nodeName();
    if        ( device=="disk" ) {
        // Hard drives, floppy disks, CDROMs
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString(tr("Disk %1")).arg(desc.toUpper()));
    } else if ( device=="interface" ) {
        // Network Interface
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString(tr("Network %1")).arg(desc.toUpper()));
    } else if ( device=="serial" ) {
        // Serial port
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString(tr("Serial Port %1")).arg(desc.toUpper()));
    } else if ( device=="parallel" ) {
        // Parallel port
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString(tr("Parallel Port %1")).arg(desc.toUpper()));
    } else if ( device=="console" ) {
        // Console
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString(tr("Console %1")).arg(desc.toUpper()));
    } else if ( device=="channel" ) {
        // Channel
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString(tr("Channel %1")).arg(desc.toUpper()));
    } else if ( device=="smartcard" ) {
        // SmartCard
        name.append(QString(tr("SmartCard %1")).arg(desc.toUpper()));
    } else if ( device=="input" ) {
        // Input
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString(tr("Input %1")).arg(desc.toUpper()));
    } else if ( device=="hub" ) {
        // Hub
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString(tr("Hub %1")).arg(desc.toUpper()));
    } else if ( device=="video" ) {
        // Video
        if (list.item(0).firstChildElement("model").attributes().contains("type"))
            desc = list.item(0).firstChildElement("model")
                    .attributes().namedItem("type").nodeValue();
        name.append(QString(tr("Video %1")).arg(desc.toUpper()));
    } else if ( device=="sound" ) {
        // Sound
        if (list.item(0).attributes().contains("model"))
            desc = list.item(0).attributes().namedItem("model").nodeValue();
        name.append(QString(tr("Sound %1")).arg(desc.toUpper()));
    } else if ( device=="hostdev" ) {
        // HostDevice
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString(tr("Host Device %1")).arg(desc.toUpper()));
    } else if ( device=="graphics" ) {
        // Graphics
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString(tr("Display %1")).arg(desc.toUpper()));
    } else if ( device=="redirdev" ) {
        // Redirected devices
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString(tr("USB Redirector %1")).arg(desc.toUpper()));
    } else if ( device=="filesystem" ) {
        // Filesystems
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString(tr("Filesystem %1")).arg(desc.toUpper()));
    } else if ( device=="controller" ) {
        // Controller
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString(tr("Controller %1")).arg(desc.toUpper()));
    } else if ( device=="emulator" ) {
        // Emulator should be unique
        QList<QListWidgetItem*> _family =
                usedDeviceList->findItems(
                    "emulator", Qt::MatchContains);
        if ( !_family.isEmpty() ) {
            QListWidgetItem *item =
                    usedDeviceList->takeItem(
                        usedDeviceList->row(_family.at(0)));
            if ( nullptr!=item ) {
                delete item;
                item = nullptr;
            };
        };
        name.append(tr("Emulator"));
    } else if ( device=="watchdog" ) {
        // WatchDog
        if (list.item(0).attributes().contains("model"))
            desc = list.item(0).attributes().namedItem("model").nodeValue();
        name.append(QString(tr("WatchDog %1")).arg(desc.toUpper()));
    } else if ( device=="memballoon" ) {
        // MemBalloon
        if (list.item(0).attributes().contains("model"))
            desc = list.item(0).attributes().namedItem("model").nodeValue();
        name.append(QString(tr("MemBalloon %1")).arg(desc.toUpper()));
    } else if ( device=="rng" ) {
        // Random
        if (list.item(0).attributes().contains("model"))
            desc = list.item(0).attributes().namedItem("model").nodeValue();
        name.append(QString(tr("RNG %1")).arg(desc.toUpper()));
    } else if ( device=="tpm" ) {
        // TPM
        if (list.item(0).attributes().contains("model"))
            desc = list.item(0).attributes().namedItem("model").nodeValue();
        name.append(QString(tr("TPM %1")).arg(desc.toUpper()));
    } else if ( device=="nvram" ) {
        // NVRAM
        name.append(tr("NVRAM"));
    } else if ( device=="panic" ) {
        // Panic
        name.append(tr("Panic"));
    } else return;
    // find DeviceName in Order
    int i = -1;
    foreach (QString _name, devNameOrder) {
        if ( name.startsWith(_name) ) {
            i = devNameOrder.indexOf(_name);
            break;
        };
    };
    // impossible case, but...
    if (i<0) return;
    // insert item by Device Name Order
    bool inserted = false;
    do {
        int row = 0;
         QList<QListWidgetItem*> _family =
                usedDeviceList->findItems(
                     devNameOrder.at(i),
                     Qt::MatchCaseSensitive | Qt::MatchStartsWith);
         if ( _family.isEmpty() ) {
             if ( i>0) {
                 --i;
                 continue;
             };
         } else {
             QListWidgetItem *lastItem = _family.last();
             row = usedDeviceList->row( lastItem ) + 1;
         };
         QListWidgetItem *item = new QListWidgetItem();
         item->setText(name);
         item->setData(Qt::UserRole, doc.toString());
         item->setIcon(QIcon::fromTheme(
                           QString("device-%1").arg(device)));
         usedDeviceList->insertItem(row, item);
         //usedDeviceList->insertItem(row, name);
         //usedDeviceList->item(row)->setData(Qt::UserRole, doc.toString());
         // don't show device here, because this metod
         // fill the device list only
         // showDevice(item, nullptr);
         inserted = true;
    } while ( !inserted );
    //qDebug()<<"added New Device:"<<name;
    // check the flag for dropping infinite loop
    if ( flag ) initBootDevices();
}
void Devices::delDevice()
{
    //qDebug()<<"Delete"<<usedDeviceList->currentItem()->text();
    infoWidget->clearDataEdit();
    QListWidgetItem *item =
            usedDeviceList->takeItem(usedDeviceList->currentRow());
    if ( nullptr!=item ) {
        delete item;
        item = nullptr;
    };
    initBootDevices();
}
void Devices::showDevice()
{
    QListWidgetItem *_curr = usedDeviceList->currentItem();
    if ( nullptr!=_curr ) showDevice( _curr, nullptr );
}
void Devices::showDevice(QListWidgetItem *_curr, QListWidgetItem *_prev)
{
    if ( nullptr!=_prev ) infoWidget->closeDataEdit();
    if ( nullptr==_curr ) return;
    QString _devName, _devDesc;
    _devName = _curr->text();
    _devDesc = _curr->data(Qt::UserRole).toString();
    int idx = usedDeviceList->row(_curr);
    infoWidget->showDevice(idx, _devName, _devDesc);
}
void Devices::showContextMenu(const QPoint &pos)
{
    usedDeviceList->clearSelection();
    QListWidgetItem *item = usedDeviceList->itemAt(pos);
    DeviceExistanceMenu *jobMenu = new DeviceExistanceMenu(this, (item!=nullptr));
    connect(jobMenu, SIGNAL(resultSign(Device_Action)),
            this, SLOT(execDevExistanceMenuResult(Device_Action)));
    jobMenu->move(mapToGlobal(pos));
    jobMenu->exec();
    disconnect(jobMenu, SIGNAL(resultSign(Device_Action)),
               this, SLOT(execDevExistanceMenuResult(Device_Action)));
    jobMenu->deleteLater();
}
void Devices::execDevExistanceMenuResult(Device_Action ret)
{
    if ( ret==ADD ) {
        stackWdg->setCurrentWidget(deviceStack);
    } else if ( ret==DEL ) {
        delDevice();
    };
}
void Devices::detectAttachedDevicesFromXMLDesc()
{
    if ( !xmlDesc.isEmpty() ) {
        // read devices from XML domain description
        QDomDocument doc;
        doc.setContent(xmlDesc);
        QDomNodeList list = doc
                .firstChildElement("domain")
                .firstChildElement("devices")
                .childNodes();
        uint j = 0;
        int count = list.length();
        for (int i=0; i<count;i++) {
            if (!list.item(j).isNull()) {
                QDomDocument _device;
                QDomElement _devDesc = _device.createElement("device");
                _devDesc.appendChild(list.item(j));
                _device.appendChild(_devDesc);
                addDeviceToUsedDevList(_device);
            } else ++j;
        };
    }
}
void Devices::saveDeviceXMLDescription(const int idx, const QString &xmlDesc)
{
    /* block/unblock usedDeviceList signals to avoid looping */
    usedDeviceList->blockSignals(true);
    if ( idx>=0 ) {
        QListWidgetItem *item = usedDeviceList->takeItem(idx);
        if ( nullptr!=item ) {
            delete item;
            item = nullptr;
        };
    };
    QDomDocument doc;
    doc.setContent(xmlDesc);
    addDeviceToUsedDevList(doc);
    usedDeviceList->blockSignals(false);
}
