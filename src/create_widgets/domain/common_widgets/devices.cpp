#include "devices.h"

/*
 * http://libvirt.org/formatdomain.html
 *
    <!- -------------------------- ->
    Devices
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
        QWidget *parent,
        virConnectPtr conn,
        virDomainPtr domain
        ) :
    _QWidget(parent, conn, domain)
{
    setObjectName("Computer:Device");
    usedDeviceList = new QListWidget(this);
    usedDeviceList->setContextMenuPolicy(Qt::CustomContextMenu);
    usedDeviceList->setSortingEnabled(true);
    connect(usedDeviceList,
            SIGNAL(customContextMenuRequested(const QPoint&)),
            this,
            SLOT(showContextMenu(const QPoint&)));
    addNewDevice = new QPushButton(QIcon::fromTheme("list-add"), "", this);
    addNewDevice->setToolTip("Add Device");
    connect(addNewDevice, SIGNAL(clicked()), this, SLOT(addDevice()));
    delSelDevice = new QPushButton(QIcon::fromTheme("list-remove"), "", this);
    delSelDevice->setToolTip("Del Device");
    connect(delSelDevice, SIGNAL(clicked()), this, SLOT(delDevice()));

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

    infoWidget = new DeviceData(this, currWorkConnect);
    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(listWidget, 3);
    commonLayout->addWidget(infoWidget, 8);
    detectAttachedDevicesFromXMLDesc();
    connect(usedDeviceList, SIGNAL(currentRowChanged(int)),
            this, SLOT(showDevice(int)));
    connect(infoWidget, SIGNAL(saveDeviceXMLDesc(QString&)),
            this, SLOT(saveDeviceXMLDescription(QString&)));
}
Devices::~Devices()
{
    if ( deviceStack!=NULL ) {
        delete deviceStack;
        deviceStack = 0;
    };
    disconnect(usedDeviceList,
               SIGNAL(customContextMenuRequested(const QPoint&)),
               this,
               SLOT(showContextMenu(const QPoint&)));
    disconnect(addNewDevice, SIGNAL(clicked()), this, SLOT(addDevice()));
    disconnect(delSelDevice, SIGNAL(clicked()), this, SLOT(delDevice()));
    delete usedDeviceList;
    usedDeviceList = 0;
    delete addNewDevice;
    addNewDevice = 0;
    delete delSelDevice;
    delSelDevice = 0;

    delete buttonlayout;
    buttonlayout = 0;
    delete buttons;
    buttons = 0;

    delete listLayout;
    listLayout = 0;
    delete listWidget;
    listWidget = 0;

    delete infoWidget;
    infoWidget = 0;
    delete commonLayout;
    commonLayout = 0;
}

/* public slots */
QDomDocument Devices::getDevDocument() const
{
    /*
     * parse usedDeviceList
     */
    //qDebug()<<"Device result";
    QDomDocument doc = QDomDocument();
    QDomElement devices = doc.createElement("devices");
    for (int i=0; i<usedDeviceList->count(); i++) {
        QDomDocument _doc = QDomDocument();
        _doc.setContent(usedDeviceList->item(i)->data(Qt::UserRole).toString());
        QDomNodeList list = _doc.firstChildElement("device").childNodes();
        int j = 0;
        uint count = list.length();
        for (uint i=0; i<count;i++) {
            //qDebug()<<list.item(j).nodeName()<<i;
            if (!list.item(j).isNull()) {
                devices.appendChild(list.item(j));
            } else ++j;
        };
    };
    doc.appendChild(devices);
    //qDebug()<<doc.toString()<<"Device result";
    return doc;
}

/* private slots */
QDomDocument Devices::chooseNewDevice()
{
    QDomDocument doc = QDomDocument();
    qDebug()<<"choose New Device";
    if ( deviceStack==NULL ) {
        deviceStack = new DeviceStack(this, currWorkConnect);
    };
    if ( deviceStack->exec()==1 ) {
        doc = deviceStack->getResult();
    };
    return doc;
}
void Devices::addDevice()
{
    QDomDocument doc = chooseNewDevice();
    //qDebug()<<doc.toString();
    QDomNodeList list = doc.firstChildElement("device").childNodes();
    if ( list.length()==0 ) return;
    QString device, desc, name;
    device = list.item(0).nodeName();
    QListWidgetItem *item = new QListWidgetItem(usedDeviceList);
    if ( device=="disk" ) {
        // Hard drives, floppy disks, CDROMs
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString("Disk %1").arg(desc.toUpper()));
    } else if ( device=="interface" ) {
        // Network Interface
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString("Network %1").arg(desc.toUpper()));
    } else if ( device=="serial" ) {
        // Serial port
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString("Serial Port %1").arg(desc.toUpper()));
    } else if ( device=="parallel" ) {
        // Parallel port
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString("Parallel Port %1").arg(desc.toUpper()));
    } else if ( device=="console" ) {
        // Console
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString("Console %1").arg(desc.toUpper()));
    } else if ( device=="channel" ) {
        // Channel
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString("Channel %1").arg(desc.toUpper()));
    } else if ( device=="smartcard" ) {
        // SmartCard
        name.append(QString("SmartCard %1").arg(desc.toUpper()));
    } else if ( device=="input" ) {
        // Input
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString("Input %1").arg(desc.toUpper()));
    } else if ( device=="hub" ) {
        // Hub
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString("Hub %1").arg(desc.toUpper()));
    } else if ( device=="video" ) {
        // Video
        if (list.item(0).firstChildElement("model").attributes().contains("type"))
            desc = list.item(0).firstChildElement("model").attributes().namedItem("type").nodeValue();
        name.append(QString("Video %1").arg(desc.toUpper()));
    } else if ( device=="sound" ) {
        // Sound
        if (list.item(0).attributes().contains("model"))
            desc = list.item(0).attributes().namedItem("model").nodeValue();
        name.append(QString("Sound %1").arg(desc.toUpper()));
    } else if ( device=="hostdev" ) {
        // HostDevice
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString("Host Device %1").arg(desc.toUpper()));
    } else if ( device=="graphics" ) {
        // Graphics
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString("Display %1").arg(desc.toUpper()));
    } else if ( device=="redirdev" ) {
        // Redirected devices
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString("USB Redirector %1").arg(desc.toUpper()));
    } else if ( device=="filesystem" ) {
        // Filesystems
        if (list.item(0).attributes().contains("type"))
            desc = list.item(0).attributes().namedItem("type").nodeValue();
        name.append(QString("Filesystem %1").arg(desc.toUpper()));
    } else return;
    item->setText(name);
    item->setData(Qt::UserRole, doc.toString());
    usedDeviceList->addItem(item);
    qDebug()<<"added New Device";
}
void Devices::delDevice()
{
    //qDebug()<<"Delete"<<usedDeviceList->currentItem()->text();
    QListWidgetItem *item = usedDeviceList->takeItem(usedDeviceList->currentRow());
    delete item;
    item = 0;
}
void Devices::showDevice()
{
    showDevice( usedDeviceList->currentRow() );
}
void Devices::showDevice(int i)
{
    QString _devName, _devDesc;
    QListWidgetItem *item = usedDeviceList->item(i);
    _devName = item->text();
    _devDesc = item->data(Qt::UserRole).toString();
    infoWidget->showDevice(_devName, _devDesc);
}
void Devices::showContextMenu(const QPoint &pos)
{
    QListWidgetItem *item = usedDeviceList->itemAt(pos);
    DeviceExistanceMenu *jobMenu = new DeviceExistanceMenu(this, (item!=NULL));
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
        addDevice();
    } else if ( ret==DEL ) {
        delDevice();
    } else if ( ret==SHOW ) {
        showDevice();
    };
}
void Devices::detectAttachedDevicesFromXMLDesc()
{
    if ( currDomain!=NULL ) {
        // TODO : read devices from XML domain description
    }
}
void Devices::saveDeviceXMLDescription(QString &xmlDesc)
{
    usedDeviceList->currentItem()->setData(Qt::UserRole, xmlDesc);
}
