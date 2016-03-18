#include "device_stack.h"

#define DEV_LIST QStringList()\
    <<"Emulator"\
    <<"Disk"<<"FileSystem"<<"Controller"<<"Host Devices"\
    <<"USB Redirection"<<"SmartCard"<<"Network"\
    <<"Input"<<"Hub"<<"Graphics"<<"Video"\
    <<"Console"<<"Serial"<<"Parallel"<<"Channel"\
    <<"Sound"<<"WatchDog"<<"Memory Balloon"\
    <<"RNG"<<"TPM"<<"NVRAM"<<"Panic Notifier"
#define DEV_TYPE QStringList()\
    <<"emulator"\
    <<"disk"<<"filesystem"<<"controller"<<"hostdev"\
    <<"redirdev"<<"smartcard"<<"interface"\
    <<"input"<<"hub"<<"graphics"<<"video"\
    <<"console"<<"serial"<<"parallel"<<"channel"\
    <<"sound"<<"watchdog"<<"memballoon"\
    <<"rng"<<"tpm"<<"nvram"<<"panic"

#define QEMU_DEVICE_LIST QStringList()\
    <<"emulator"\
    <<"disk"<<"filesystem"<<"hostdev"\
    <<"redirdev"<<"smartcard"<<"interface"\
    <<"input"<<"hub"<<"graphics"<<"video"\
    <<"console"<<"serial"<<"parallel"<<"channel"\
    <<"sound"<<"watchdog"<<"memballoon"<<"rng"\
    <<"tpm"<<"nvram"<<"panic"

#define LXC_DEVICE_LIST QStringList()\
    <<"emulator"\
    <<"disk"<<"filesystem"<<"hostdev"\
    <<"smartcard"<<"interface"<<"input"\
    <<"hub"<<"graphics"<<"video"\
    <<"console"<<"serial"<<"parallel"<<"channel"\
    <<"sound"<<"rng"<<"nvram"

#define XEN_DEVICE_LIST QStringList()\
    <<"emulator"\
    <<"disk"<<"filesystem"<<"controller"<<"hostdev"\
    <<"smartcard"<<"interface"\
    <<"input"<<"hub"<<"graphics"<<"video"\
    <<"console"<<"serial"<<"parallel"<<"channel"\
    <<"sound"<<"memballoon"\
    <<"rng"<<"nvram"

#define VBOX_DEVICE_LIST QStringList()\
    <<"emulator"\
    <<"disk"<<"filesystem"<<"controller"<<"hostdev"\
    <<"smartcard"<<"interface"\
    <<"input"<<"hub"<<"graphics"<<"video"\
    <<"console"<<"serial"<<"parallel"<<"channel"\
    <<"sound"<<"rng"<<"nvram"

#define VMWARE_DEVICE_LIST QStringList()\
    <<"emulator"\
    <<"disk"<<"filesystem"<<"controller"<<"hostdev"\
    <<"smartcard"<<"interface"\
    <<"input"<<"hub"<<"graphics"<<"video"\
    <<"console"<<"serial"<<"parallel"<<"channel"\
    <<"sound"<<"rng"<<"nvram"

#define OPENVZ_DEVICE_LIST QStringList()\
    <<"emulator"\
    <<"disk"<<"filesystem"<<"controller"<<"hostdev"\
    <<"smartcard"<<"interface"\
    <<"input"<<"hub"<<"graphics"<<"video"\
    <<"console"<<"serial"<<"parallel"<<"channel"\
    <<"sound"<<"rng"<<"nvram"

DeviceStack::DeviceStack(
        QWidget *parent, virConnectPtr *conn) :
    QDialog(parent), ptr_ConnPtr(conn)
{
    infoLayout = new QVBoxLayout(this);
    infoWidget = new QScrollArea(this);
    scrolled = new QWidget(this);
    scrolled->setLayout(infoLayout);
    infoWidget->setWidget(scrolled);
    infoWidget->setWidgetResizable(true);
    QString connType;
    if ( nullptr!=ptr_ConnPtr && nullptr!=*ptr_ConnPtr ) {
        // TODO: reimplement for using in thread
        // and in another same widgets
        connType = QString(virConnectGetType(*ptr_ConnPtr)).toLower();
    } else
        emit ptrIsNull();
    QStringList devSet, devList, devType;
    devList = DEV_LIST;
    devType = DEV_TYPE;
    if ( connType=="qemu" ) {
        devSet = QEMU_DEVICE_LIST;
    } else if ( connType=="lxc" ) {
        devSet = LXC_DEVICE_LIST;
    } else if ( connType=="xen" ) {
        devSet = XEN_DEVICE_LIST;
    } else if ( connType=="vbox" ) {
        devSet = VBOX_DEVICE_LIST;
    } else if ( connType=="vbox" ) {
        devSet = VMWARE_DEVICE_LIST;
    } else if ( connType=="vbox" ) {
        devSet = OPENVZ_DEVICE_LIST;
    };
    deviceList = new QListWidget(this);
    deviceList->setSortingEnabled(false);
    /* set icons & user data */
    for (int i=0; i<devList.count();i++) {
        if ( devSet.contains(devType.at(i)) ) {
            deviceList->addItem(devList.at(i));
            QListWidgetItem *item =
                    deviceList->item(
                        deviceList->count()-1);
            /*
            item->setIcon(
                  QIcon::fromTheme(
                         item->text()
                         .split(" ")
                         .first()
                         .toLower()));
             */
            item->setData(
                        Qt::UserRole,
                        QVariant(devType.at(i)));
            //qDebug()<<item->text();
        };
    };

    connect(deviceList, SIGNAL(itemSelectionChanged()),
            this, SLOT(showDevice()));
    listLayout = new QHBoxLayout(this);
    listLayout->addWidget(deviceList, 3);
    listLayout->addWidget(infoWidget, 8);
    listWidget = new QWidget(this);
    listWidget->setLayout(listLayout);

    addDevice = new QPushButton(QIcon::fromTheme("dialog-ok"), "Add Device", this);
    cancel = new QPushButton(QIcon::fromTheme("dialog-cancel"), "Cancel", this);
    connect(addDevice, SIGNAL(clicked()),
            this, SLOT(set_Result()));
    connect(cancel, SIGNAL(clicked()),
            this, SLOT(set_Result()));
    buttonlayout = new QHBoxLayout(this);
    buttonlayout->addWidget(addDevice);
    buttonlayout->addWidget(cancel);
    buttons = new QWidget(this);
    buttons->setLayout(buttonlayout);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(listWidget);
    commonLayout->addWidget(buttons);
    setLayout(commonLayout);
}

/* public slots */
QDomDocument DeviceStack::getResult() const
{
    //qDebug()<<"DeviceStack result";
    QDomDocument doc;
    if ( device!=nullptr ) {
        doc = device->getDataDocument();
    };
    return doc;
}
void DeviceStack::clearDevice()
{
    if ( device!=nullptr ) {
        infoLayout->removeWidget(device);
        delete device;
        device = nullptr;
    };
}

/* private slots */
void DeviceStack::showDevice(QListWidgetItem *item)
{
    if ( item==nullptr ) {
        deviceDataProcessed();
        return;
    };
    clearDevice();
    QString deviceType = item->data(Qt::UserRole).toString();
    //qDebug()<<item->text()<<deviceType;
    if ( deviceType == "disk" ) {
        device = new Disk(
                    this,
                    ptr_ConnPtr);
        connect(device, SIGNAL(errorMsg(QString&)),
                this, SIGNAL(errorMsg(QString&)));
    } else if ( deviceType == "interface" ) {
        device = new NetInterfaces(
                    this,
                    ptr_ConnPtr);
    } else if ( deviceType == "serial" ) {
        device = new CharDevice(
                    this,
                    nullptr,
                    nullptr,
                    deviceType);
    } else if ( deviceType == "parallel" ) {
        device = new CharDevice(
                    this,
                    nullptr,
                    nullptr,
                    deviceType);
    } else if ( deviceType == "channel" ) {
        device = new ChannelDevice(this);
    } else if ( deviceType == "console" ) {
        device = new ConsoleDevice(
                    this,
                     ptr_ConnPtr);
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
                    ptr_ConnPtr);
    } else if ( deviceType == "graphics" ) {
        device = new GraphicsDevice(
                    this,
                    ptr_ConnPtr);
    } else if ( deviceType == "redirdev" ) {
        device = new RedirDevDevice(
                    this,
                    ptr_ConnPtr);
    } else if ( deviceType == "filesystem" ) {
        device = new FileSystems(
                    this,
                    ptr_ConnPtr);
    } else if ( deviceType == "emulator" ) {
        device = new _QWidget(this);
        //device = new Emulator(
        //            this,
        //            ptr_ConnPtr);
    } else if ( deviceType == "rng" ) {
        device = new Random(this);
    } else if ( deviceType == "memballoon" ) {
        device = new MemBalloon(
                    this,
                    ptr_ConnPtr);
    } else {
        device = new _QWidget(this);
    };
    connect(device, SIGNAL(complete()),
            this, SLOT(deviceDataProcessed()));
    infoLayout->insertWidget(0, device, -1);
}
void DeviceStack::showDevice()
{
    listWidget->setEnabled(false);
    showDevice(deviceList->currentItem());
}
void DeviceStack::set_Result()
{
    setResult( (sender()==addDevice)?
                   QDialog::Accepted : QDialog::Rejected );
    done(result());
    //qDebug()<<"done";
}
void DeviceStack::deviceDataProcessed()
{
    listWidget->setEnabled(true);
}
