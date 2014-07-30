#include "create_virt_domain.h"
#define TIMEOUT     60*1000
#define PERIOD      333

/*
 * http://libvirt.org/formatdomain.htm
 *
    General metadata
    Operating system booting
        BIOS bootloader
        Host bootloader
        Direct kernel boot
        Container boot
    SMBIOS System Information
    CPU Allocation
    CPU Tuning
    Memory Allocation
    Memory Backing
    Memory Tuning
    NUMA Node Tuning
    Block I/O Tuning
    Resource partitioning
    CPU model and topology
    Events configuration
    Power Management
    Hypervisor features
    Time keeping
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
    Security label

*/

CreateVirtDomain::CreateVirtDomain(QWidget *parent, virConnectPtr conn) :
    QDialog(parent), currWorkConnect(conn)
{
    setModal(true);
    setWindowTitle("Domain Settings");
    restoreGeometry(settings.value("DomCreateGeometry").toByteArray());
    xml = new QTemporaryFile(this);
    xml->setAutoRemove(false);
    xml->setFileTemplate(QString("%1%2XML_Desc-XXXXXX.xml").arg(QDir::tempPath()).arg(QDir::separator()));
    setEnabled(false);
    timerId = startTimer(PERIOD);
    readyDataLists();
}
CreateVirtDomain::~CreateVirtDomain()
{
    if ( timerId>0 ) killTimer(timerId);
    settings.setValue("DomCreateGeometry", saveGeometry());
    delete xml;
    xml = 0;
    if ( ready ) {
        disconnect(ok, SIGNAL(clicked()), this, SLOT(set_Result()));
        disconnect(cancel, SIGNAL(clicked()), this, SLOT(set_Result()));
        delete_specified_widgets();
        delete ok;
        ok = 0;
        delete cancel;
        cancel = 0;
        delete buttonLayout;
        buttonLayout = 0;
        delete buttons;
        buttons = 0;
        delete commonLayout;
        commonLayout = 0;
    };
}

/* public slots */
QString CreateVirtDomain::getXMLDescFileName() const
{
    return xml->fileName();
}

/* private slots */
void CreateVirtDomain::readCapabilities()
{
    QString capabilities;
    capabilities = QString("%1").arg(virConnectGetCapabilities(currWorkConnect));
    //qDebug()<<capabilities;
    QDomDocument doc = QDomDocument();
    doc.setContent(capabilities);
    arch = doc.firstChildElement("capabilities").
               firstChildElement("host").
               firstChildElement("cpu").
               firstChildElement("arch").
               firstChild().toText().data();
    os_type = doc.firstChildElement("capabilities").
               firstChildElement("guest").
               firstChildElement("os_type").
               firstChild().toText().data();
    type = doc.firstChildElement("capabilities").
               firstChildElement("guest").
               firstChildElement("arch").
               firstChildElement("domain").
               attribute("type", "???");
    memValue = doc.firstChildElement("capabilities").
               firstChildElement("host").
               firstChildElement("topology").
               firstChildElement("cells").
               firstChildElement("cell").
               firstChildElement("memory").
               firstChild().toText().data();
    memUnit = doc.firstChildElement("capabilities").
               firstChildElement("host").
               firstChildElement("topology").
               firstChildElement("cells").
               firstChildElement("cell").
               firstChildElement("memory").
               attribute("unit", "???");
    /*  search emulator from guest with system achitecture  */
    QDomElement el = doc.firstChildElement("capabilities").
               firstChildElement("guest");
    while ( !el.isNull() ) {
        QDomElement _el = el.firstChildElement("arch");
        if ( !_el.isNull() && arch==_el.attribute("name", "???") ) {
               emulator = _el.
                        firstChildElement("emulator").
                        firstChild().toText().data();
                break;
        } else el = el.nextSiblingElement("guest");
    };
    qDebug()<<arch<<os_type<<type<<emulator<<memUnit<<memValue;
}
void CreateVirtDomain::readNetworkList()
{
    virNetworkPtr *networks = NULL;
    unsigned int flags = VIR_CONNECT_LIST_NETWORKS_ACTIVE |
                         VIR_CONNECT_LIST_NETWORKS_INACTIVE;
    int ret = virConnectListAllNetworks(currWorkConnect, &networks, flags);
    if ( ret<0 ) {
        sendConnErrors();
    } else {
        int i = 0;
        while ( networks[i] != NULL ) {
            nets.append( virNetworkGetName(networks[i]) );
            virNetworkFree(networks[i]);
            i++;
        };
    };
    free(networks);
}
void CreateVirtDomain::readNodeDevicesList()
{
    int i = 0;
    if ( currWorkConnect!=NULL ) {
        unsigned int flags =
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_SYSTEM |
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_PCI_DEV |
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_USB_DEV |
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_USB_INTERFACE |
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_NET |
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_SCSI_HOST |
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_SCSI_TARGET |
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_SCSI |
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_STORAGE |
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_FC_HOST |
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_VPORTS |
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_SCSI_GENERIC;
        int ret = virConnectListAllNodeDevices(currWorkConnect, &nodeDevices, flags);
        if ( ret<0 ) {
            sendConnErrors();
        } else {
            while ( nodeDevices[i] != NULL ) {
                QString caps;
                char *names[100];
                ret = virNodeDeviceListCaps(nodeDevices[i], names, 100);
                //qDebug()<<ret<<"caps number";
                if ( ret<0 ) {
                    sendConnErrors();
                    caps.append(ret);
                } else {
                    int j = 0;
                    while (j<ret) {
                        caps.append(names[j]);
                        caps.append(" ");
                        j++;
                    };
                };
                devices.append( QString("%1\n\t\t(%2)\n\t\t\t(%3)\n(%4)")
                                .arg(virNodeDeviceGetName(nodeDevices[i]))
                                .arg(virNodeDeviceGetParent(nodeDevices[i]))
                                .arg(caps)
                                .arg(virNodeDeviceGetXMLDesc(nodeDevices[i], 0)));
                virNodeDeviceFree(nodeDevices[i]);
                i++;
            };
        };
        free(nodeDevices);
    };
    //int devs = virNodeNumOfDevices(currWorkConnect, NULL, 0);
    //qDebug()<<"Devices("<<devs<<i<<"):\n\t"<<devices.join("\n\t");
}
void CreateVirtDomain::readyDataLists()
{
    readCapabilities();
    readNetworkList();
    readNodeDevicesList();
    ready = true;
}
void CreateVirtDomain::timerEvent(QTimerEvent *ev){
    if ( timerId>0 && timerId==ev->timerId() ) {
        counter++;
        if ( ready ) {
            killTimer(timerId);
            timerId = 0;

            commonLayout = new QVBoxLayout(this);
            create_specified_widgets();
            set_specified_Tabs();
            ok = new QPushButton("Ok", this);
            ok->setAutoDefault(true);
            connect(ok, SIGNAL(clicked()), this, SLOT(set_Result()));
            cancel = new QPushButton("Cancel", this);
            cancel->setAutoDefault(true);
            connect(cancel, SIGNAL(clicked()), this, SLOT(set_Result()));
            buttonLayout = new QHBoxLayout();
            buttonLayout->addWidget(ok);
            buttonLayout->addWidget(cancel);
            buttons = new QWidget(this);
            buttons->setLayout(buttonLayout);
            commonLayout->addWidget(tabWidget);
            commonLayout->addWidget(buttons);
            setLayout(commonLayout);
            setEnabled(true);
        } else if ( TIMEOUT<counter*PERIOD ) {
            killTimer(timerId);
            timerId = 0;
            counter = 0;
            emit errorMsg( "Read Data failed." );
            // to done()
            set_Result();
        };
    };
}
void CreateVirtDomain::buildXMLDescription()
{
    /*
     * build Device description
     * then common description append it
     */
    QDomDocument doc = QDomDocument();
    QDomElement root, devices, _emulator, console, target;
    root = doc.createElement("domain");
    root.setAttribute("type", type.toLower());
    doc.appendChild(root);
    devices = doc.createElement("devices");
    WidgetList::const_iterator Wdg;
    for (Wdg=wdgList.constBegin(); Wdg!=wdgList.constEnd(); Wdg++) {
        QString property = (*Wdg)->objectName().split(":").last();
        QDomNodeList list;
        if ( property=="Device" ) {
            list = (*Wdg)->getNodeList();
        } else {
            continue;
        };
        /*
         * current DomNode is removed to root-element
         * but NULL-elemens not removed
         * therefore keep to seek on not-NULL next element
         */
        uint j = 0;
        uint count = list.length();
        for (uint i=0; i<=count;i++) {
            //qDebug()<<list.item(j).nodeName()<<i;
            if (!list.item(j).isNull()) devices.appendChild(list.item(j));
            else ++j;
        };
    };
    for (Wdg=wdgList.constBegin(); Wdg!=wdgList.constEnd(); Wdg++) {
        QString property = (*Wdg)->objectName().split(":").last();
        QDomNodeList list;
        if ( property=="Device" ) {
            continue;
        } else {
            list = (*Wdg)->getNodeList();
        };
        /*
         * current DomNode is removed to root-element
         * but NULL-elemens not removed
         * therefore keep to seek on not-NULL next element
         */
        uint j = 0;
        uint count = list.length();
        for (uint i=0; i<=count;i++) {
            //qDebug()<<list.item(j).nodeName()<<i;
            if (!list.item(j).isNull()) root.appendChild(list.item(j));
            else ++j;
        };
    };
    // append Device description
    // TODO: remove it into some Device-desc widget
    _emulator = doc.createElement("emulator");
    QDomText data = doc.createTextNode(emulator);
    _emulator.appendChild(data);
    devices.appendChild(_emulator);
    console = doc.createElement("console");
    console.setAttribute("type", "pty");
    devices.appendChild(console);
    target = doc.createElement("target");
    target.setAttribute("type", type.toLower());
    target.setAttribute("port", "0");
    console.appendChild(target);
    root.appendChild(devices);
    //
    //qDebug()<<doc.toString();

    bool read = xml->open();
    if (read) xml->write(doc.toByteArray(4).data());
    xml->close();
}
void CreateVirtDomain::set_Result()
{
    if ( sender()==ok ) {
        setResult(QDialog::Accepted);
        buildXMLDescription();
    } else {
        setResult(QDialog::Rejected);
    };
    done(result());
}
void CreateVirtDomain::create_specified_widgets()
{
    if ( type.toLower() == "lxc" ) {
        wdgList.append(new General(this, type, arch, emulator));
        wdgList.append(new LXC_OSBooting(this, os_type, arch));
        wdgList.append(new Memory(this, memUnit, memValue));
        wdgList.append(new CPU(this));
        wdgList.append(new LXC_NetInterface(this, nets));
        wdgList.append(new Devices(this));
    } else if ( type.toLower() == "qemu" ) {
        wdgList.append(new General(this, type, arch, emulator));
    } else if ( type.toLower() == "xen" ) {
        wdgList.append(new General(this, type, arch, emulator));
    } else wdgList.clear();
}
void CreateVirtDomain::set_specified_Tabs()
{
    tabWidget = new QTabWidget(this);
    WidgetList::const_iterator Wdg;
    for (Wdg=wdgList.constBegin(); Wdg!=wdgList.constEnd(); Wdg++) {
        if ( NULL!=*Wdg ) {
            qDebug()<<(*Wdg)->objectName();
            QString name = (*Wdg)->objectName().split(":").first();
            tabWidget->addTab(*Wdg, QIcon::fromTheme(name.toLower()), name);
        };
    };
}
void CreateVirtDomain::delete_specified_widgets()
{
    WidgetList::const_iterator Wdg;
    for (Wdg=wdgList.constBegin(); Wdg!=wdgList.constEnd(); Wdg++) {
        if ( NULL!=*Wdg ) {
            delete *Wdg;
        };
    };
    wdgList.clear();
    delete tabWidget;
    tabWidget = 0;
}

void CreateVirtDomain::sendConnErrors()
{
    virtErrors = virConnGetLastError(currWorkConnect);
    if ( virtErrors!=NULL && virtErrors->code>0 ) {
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                       .arg(QString().fromUtf8(virtErrors->message)) );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void CreateVirtDomain::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL && virtErrors->code>0 )
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                       .arg(QString().fromUtf8(virtErrors->message)) );
    virResetLastError();
}
