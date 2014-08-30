#include "device_stack.h"
#define DEV_LIST QStringList()\
    <<"Disk"<<"FileSystem"<<"Controller"<<"Host Devices"\
    <<"USB Redirection"<<"SmartCard"<<"Network"\
    <<"Input"<<"Hub"<<"Graphics"<<"Video"\
    <<"Console"<<"Serial"<<"Parallel"<<"Channel"\
    <<"Sound"<<"WatchDog"<<"Memory Balloon"\
    <<"RNG"<<"TPM"<<"NVRAM"<<"Panic Notifier"
#define DEV_TYPE QStringList()\
    <<"disk"<<"filesystem"<<"controller"<<"hostdev"\
    <<"redirdev"<<"smartcard"<<"interface"\
    <<"input"<<"hub"<<"graphics"<<"video"\
    <<"console"<<"serial"<<"parallel"<<"channel"\
    <<"sound"<<"watchdog"<<"memballoon"\
    <<"rng"<<"tpm"<<"nvram"<<"panic"

DeviceStack::DeviceStack(
        QWidget *parent,
        virConnectPtr conn) :
    QDialog(parent), currWorkConnect(conn)
{
    setModal(true);
    restoreGeometry(settings.value("DeviceStackGeometry").toByteArray());
    infoLayout = new QVBoxLayout(this);
    infoWidget = new QScrollArea(this);
    scrolled = new QWidget(this);
    scrolled->setLayout(infoLayout);
    infoWidget->setWidget(scrolled);
    infoWidget->setWidgetResizable(true);
    deviceList = new QListWidget(this);
    deviceList->addItems(DEV_LIST);
    /* set icons & user data */
    QStringList l = DEV_TYPE;
    for (int i=0; i<deviceList->count();i++) {
        QListWidgetItem *item = deviceList->item(i);
        /*
        item->setIcon(
              QIcon::fromTheme(
                     item->text()
                     .split(" ")
                     .first()
                     .toLower()));
         */
        item->setData(Qt::UserRole, QVariant(l.at(i)));
        //qDebug()<<item->text();
    };

    // double event for selection item
    //connect(deviceList, SIGNAL(itemClicked(QListWidgetItem*)),
    //        this, SLOT(showDevice(QListWidgetItem*)));
    connect(deviceList, SIGNAL(itemSelectionChanged()),
            this, SLOT(showDevice()));
    listLayout = new QHBoxLayout(this);
    listLayout->addWidget(deviceList, 3);
    listLayout->addWidget(infoWidget, 8);
    listWidget = new QWidget(this);
    listWidget->setLayout(listLayout);

    addDevice = new QPushButton(QIcon::fromTheme("dialog-ok"), "Add Device", this);
    cancel = new QPushButton(QIcon::fromTheme("dialog-cancel"), "Cancel", this);
    connect(addDevice, SIGNAL(clicked()), this, SLOT(set_Result()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(set_Result()));
    buttonlayout = new QHBoxLayout(this);
    buttonlayout->addWidget(addDevice);
    buttonlayout->addWidget(cancel);
    buttons = new QWidget(this);
    buttons->setLayout(buttonlayout);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(listWidget);
    commonLayout->addWidget(buttons);
    setLayout(commonLayout);
    readNetworkList();
    readNodeDevicesList();
}
DeviceStack::~DeviceStack()
{
    //disconnect(deviceList, SIGNAL(itemClicked(QListWidgetItem*)),
    //           this, SLOT(showDevice(QListWidgetItem*)));
    disconnect(deviceList, SIGNAL(itemSelectionChanged()),
               this, SLOT(showDevice()));
    disconnect(addDevice, SIGNAL(clicked()), this, SLOT(set_Result()));
    disconnect(cancel, SIGNAL(clicked()), this, SLOT(set_Result()));
    delete deviceList;
    deviceList = 0;

    delete infoLayout;
    infoLayout = 0;
    if ( scrolled!=NULL ) {
        delete scrolled;
        scrolled = NULL;
    };
    delete infoWidget;
    infoWidget = 0;

    delete listLayout;
    listLayout = 0;
    delete listWidget;
    listWidget = 0;

    delete addDevice;
    addDevice = 0;
    delete cancel;
    cancel = 0;

    delete buttonlayout;
    buttonlayout = 0;
    delete buttons;
    buttons = 0;

    delete commonLayout;
    commonLayout = 0;
}

/* public slots */
QDomDocument DeviceStack::getResult() const
{
    qDebug()<<"DeviceStack result";
    QDomDocument doc;
    if ( device!=NULL ) {
        doc = device->getDevDocument();
    };
    return doc;
}

/* private slots */
void DeviceStack::readNetworkList()
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
void DeviceStack::readNodeDevicesList()
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
}
void DeviceStack::showDevice(QListWidgetItem *item)
{
    if ( device!=NULL ) {
        infoLayout->removeWidget(device);
        delete device;
        device = NULL;
    };
    QString deviceType = item->data(Qt::UserRole).toString();
    qDebug()<<item->text()<<deviceType;
    // TODO: display devices available for current driver
    if ( deviceType == "interface" ) {
        device = new LXC_NetInterface(this, nets);
    } else if ( deviceType == "serial" ) {
        device = new CharDevice(this, NULL, NULL, deviceType);
    } else if ( deviceType == "parallel" ) {
        device = new CharDevice(this, NULL, NULL, deviceType);
    } else if ( deviceType == "channel" ) {
        device = new ChannelDevice(this);
    } else if ( deviceType == "console" ) {
        device = new ConsoleDevice(
                          this,
                          currWorkConnect);
    } else if ( deviceType == "smartcard" ) {
        device = new SmartCardDevice(this);
    } else if ( deviceType == "input" ) {
        device = new InputDevice(this);
    } else if ( deviceType == "hub" ) {
        device = new HubDevice(this);
    } else if ( deviceType == "video" ) {
        device = new VideoDevice(this);
    } else if ( deviceType == "sound" ) {
        device = new SoundDevice(this);
    } else if ( deviceType == "hostdev" ) {
        device = new HostDevice(
                          this,
                          currWorkConnect);
    } else if ( deviceType == "graphics" ) {
        device = new GraphicsDevice(
                    this,
                    currWorkConnect);
    } else {
        device = new _QWidget(this);
    };
    infoLayout->insertWidget(0, device, -1);
}
void DeviceStack::showDevice()
{
    showDevice(deviceList->currentItem());
}
void DeviceStack::set_Result()
{
    done( (sender()==addDevice)?1:0 );
    qDebug()<<"done";
    settings.setValue("DeviceStackGeometry", saveGeometry());
}

void DeviceStack::sendConnErrors()
{
    virtErrors = virConnGetLastError(currWorkConnect);
    if ( virtErrors!=NULL && virtErrors->code>0 ) {
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                       .arg(QString().fromUtf8(virtErrors->message)) );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void DeviceStack::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL && virtErrors->code>0 )
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                       .arg(QString().fromUtf8(virtErrors->message)) );
    virResetLastError();
}
