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
    infoWidget->setLayout(infoLayout);
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

    wdgMap.clear();
}

/* public slots */
QDomNodeList DeviceStack::getResult() const
{
    qDebug()<<"DeviceStack result";
    if ( currDeviceType.isEmpty() ) {
        return QDomNodeList();
    } else if (wdgMap.value(currDeviceType)!=NULL) {
        QDomNodeList list = wdgMap.value(currDeviceType)->getNodeList();
        QDomDocument doc = QDomDocument();
        QDomElement _device = doc.createElement(currDeviceType);
        if ( !wdgMap.value(currDeviceType)->getDevType().isEmpty() )
            _device.setAttribute("type", wdgMap.value(currDeviceType)->getDevType());
        if ( !wdgMap.value(currDeviceType)->getDevMode().isEmpty() )
            _device.setAttribute("mode", wdgMap.value(currDeviceType)->getDevMode());
        if ( !wdgMap.value(currDeviceType)->getDevBus().isEmpty() )
            _device.setAttribute("bus", wdgMap.value(currDeviceType)->getDevBus());
        if ( !wdgMap.value(currDeviceType)->getDevModel().isEmpty() )
            _device.setAttribute("model", wdgMap.value(currDeviceType)->getDevModel());
        if ( !wdgMap.value(currDeviceType)->getDevManaged().isEmpty() )
            _device.setAttribute("managed", wdgMap.value(currDeviceType)->getDevManaged());
        if ( !wdgMap.value(currDeviceType)->getDevDisplay().isEmpty() )
            _device.setAttribute("display", wdgMap.value(currDeviceType)->getDevDisplay());
        if ( !wdgMap.value(currDeviceType)->getDevXauth().isEmpty() )
            _device.setAttribute("xauth", wdgMap.value(currDeviceType)->getDevXauth());
        if ( !wdgMap.value(currDeviceType)->getDevFullScreen().isEmpty() )
            _device.setAttribute("fullscreen", wdgMap.value(currDeviceType)->getDevFullScreen());
        // check valid Node
        if ( !_device.attributes().isEmpty() ) doc.appendChild(_device);
        else if ( list.count()>0 ) doc.appendChild(_device);

        uint j = 0;
        uint count = list.length();
        for (uint i=0; i<count;i++) {
            //qDebug()<<list.item(j).nodeName()<<i;
            if (!list.item(j).isNull()) _device.appendChild(list.item(j));
            else ++j;
        };

        //qDebug()<<doc.toString();
        return doc.childNodes();
    } else
        return QDomNodeList();
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
    qDebug()<<item->text();
    infoWidget->layout()->removeWidget(wdgMap.value(currDeviceType));
    delete wdgMap.value(currDeviceType);
    wdgMap.insert(currDeviceType, NULL);
    wdgMap.remove(currDeviceType);
    currDeviceType.clear();
    currDeviceType = item->data(Qt::UserRole).toString();
    // TODO: display devices available for current driver
    if ( item->data(Qt::UserRole) == "interface" ) {
        wdgMap.insert(currDeviceType, new LXC_NetInterface(this, nets));
    } else if ( currDeviceType == "serial" ) {
        wdgMap.insert(currDeviceType, new CharDevice(this));
    } else if ( currDeviceType == "parallel" ) {
        wdgMap.insert(currDeviceType, new CharDevice(this));
    } else if ( currDeviceType == "channel" ) {
        wdgMap.insert(currDeviceType, new ChannelDevice(this));
    } else if ( currDeviceType == "console" ) {
        wdgMap.insert(currDeviceType, new ConsoleDevice(
                          this,
                          currWorkConnect));
    } else if ( currDeviceType == "smartcard" ) {
        wdgMap.insert(currDeviceType, new SmartCardDevice(this));
    } else if ( currDeviceType == "input" ) {
        wdgMap.insert(currDeviceType, new InputDevice(this));
    } else if ( currDeviceType == "hub" ) {
        wdgMap.insert(currDeviceType, new HubDevice(this));
    } else if ( currDeviceType == "video" ) {
        wdgMap.insert(currDeviceType, new VideoDevice(this));
    } else if ( currDeviceType == "sound" ) {
        wdgMap.insert(currDeviceType, new SoundDevice(this));
    } else if ( currDeviceType == "hostdev" ) {
        wdgMap.insert(currDeviceType, new HostDevice(
                          this,
                          currWorkConnect));
    } else if ( currDeviceType == "graphics" ) {
        wdgMap.insert(currDeviceType, new GraphicsDevice(this));
    } else {
        wdgMap.insert(currDeviceType, new _QWidget(this));
    };
    infoWidget->layout()->addWidget(wdgMap.value(currDeviceType));
}
void DeviceStack::showDevice()
{
    showDevice(deviceList->currentItem());
}
void DeviceStack::set_Result()
{
    setResult( (sender()==addDevice)?1:0 );
    done(result());
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
