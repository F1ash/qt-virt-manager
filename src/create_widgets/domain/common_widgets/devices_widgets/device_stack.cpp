#include "device_stack.h"
#include "create_widgets/domain/common_widgets/char_device.h"
#include "create_widgets/domain/common_widgets/channel_device.h"
#include "create_widgets/domain/common_widgets/console_device.h"
#include "create_widgets/domain/common_widgets/smartcard_device.h"
#include "create_widgets/domain/common_widgets/input_device.h"
#include "create_widgets/domain/common_widgets/hub_device.h"
#include "create_widgets/domain/common_widgets/video_device.h"
#include "create_widgets/domain/common_widgets/sound_device.h"
#include "create_widgets/domain/common_widgets/host_device.h"
#include "create_widgets/domain/common_widgets/graphics_device.h"
#include "create_widgets/domain/common_widgets/redirdev_device.h"
#include "create_widgets/domain/common_widgets/filesystems.h"
#include "create_widgets/domain/common_widgets/net_interfaces.h"
#include "create_widgets/domain/common_widgets/disk.h"
#include "create_widgets/domain/common_widgets/encryption.h"
#include "create_widgets/domain/common_widgets/emulator.h"
#include "create_widgets/domain/common_widgets/random.h"
#include "create_widgets/domain/common_widgets/memballoon.h"

devstackHelpThread::devstackHelpThread(
        QObject         *parent,
        virConnectPtr   *connPtrPtr) :
    _VirtThread(parent, connPtrPtr)
{

}
void devstackHelpThread::run()
{
    if ( nullptr==ptr_ConnPtr || nullptr==*ptr_ConnPtr ) {
        emit ptrIsNull();
        return;
    };
    if ( virConnectRef(*ptr_ConnPtr)<0 ) {
        sendConnErrors();
        return;
    };
    connType = QString::fromUtf8(
                virConnectGetType(*ptr_ConnPtr))
            .toLower();
    if ( virConnectClose(*ptr_ConnPtr)<0 )
        sendConnErrors();
}

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
    deviceList = new QListWidget(this);
    deviceList->setSortingEnabled(false);
    listLayout = new QHBoxLayout(this);
    listLayout->addWidget(deviceList, 3);
    listLayout->addWidget(infoWidget, 8);
    listWidget = new QWidget(this);
    listWidget->setLayout(listLayout);

    addDevice = new QPushButton(
                QIcon::fromTheme("dialog-ok"),
                tr("Add Device"),
                this);
    cancel = new QPushButton(
                QIcon::fromTheme("dialog-cancel"),
                tr("Cancel"),
                this);
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
    hlpThread = new devstackHelpThread(this, ptr_ConnPtr);
    connect(hlpThread, SIGNAL(finished()),
            this, SLOT(init_wdg()));
    hlpThread->start();
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
    if ( devIcon!=nullptr ) {
        infoLayout->removeWidget(devIcon);
        delete devIcon;
        devIcon = nullptr;
    };
}

/* private slots */
void DeviceStack::init_wdg()
{
    QStringList devSet, devList, devType;
    devList = DEV_LIST;
    devType = DEV_TYPE;
    if ( hlpThread->connType.toLower().compare("qemu")==0 ) {
        devSet = QEMU_DEVICE_LIST;
    } else if ( hlpThread->connType.toLower().compare("lxc")==0 ) {
        devSet = LXC_DEVICE_LIST;
    } else if ( hlpThread->connType.toLower().compare("xen")==0 ) {
        devSet = XEN_DEVICE_LIST;
    } else if ( hlpThread->connType.toLower().compare("vbox")==0 ) {
        devSet = VBOX_DEVICE_LIST;
    } else if ( hlpThread->connType.toLower().compare("vmware")==0 ) {
        devSet = VMWARE_DEVICE_LIST;
    } else if ( hlpThread->connType.toLower().compare("openvz")==0 ) {
        devSet = OPENVZ_DEVICE_LIST;
    };
    /* set icons & user data */
    for (int i=0; i<devList.count();i++) {
        if ( devSet.contains(devType.at(i)) ) {
            deviceList->addItem(devList.at(i));
            QListWidgetItem *item =
                    deviceList->item(
                        deviceList->count()-1);
            item->setIcon(
                  QIcon::fromTheme(
                            QString("device-%1")
                            .arg(devType.at(i))));
            item->setData(
                        Qt::UserRole,
                        QVariant(devType.at(i)));
            //qDebug()<<item->text();
        };
    };

    connect(deviceList, SIGNAL(itemSelectionChanged()),
            this, SLOT(showDevice()));
}
void DeviceStack::showDevice(QListWidgetItem *item)
{
    if ( item==nullptr ) {
        deviceDataProcessed();
        return;
    };
    clearDevice();
    QString deviceType = item->data(Qt::UserRole).toString();
    //qDebug()<<item->text()<<deviceType;
    devIcon = new QLabel(this);
    devIcon->setPixmap(QIcon::fromTheme(
                           QString("device-%1").arg(deviceType))
                       .pixmap(64));
    if        ( deviceType.compare("disk")==0 ) {
        device = new Disk(
                    this,
                    ptr_ConnPtr);
    } else if ( deviceType.compare("interface")==0 ) {
        device = new NetInterfaces(
                    this,
                    ptr_ConnPtr);
    } else if ( deviceType.compare("serial")==0 ) {
        device = new CharDevice(
                    this,
                    nullptr,
                    nullptr,
                    deviceType);
    } else if ( deviceType.compare("parallel")==0 ) {
        device = new CharDevice(
                    this,
                    nullptr,
                    nullptr,
                    deviceType);
    } else if ( deviceType.compare("channel")==0 ) {
        device = new ChannelDevice(this);
    } else if ( deviceType.compare("console")==0 ) {
        device = new ConsoleDevice(
                    this,
                     ptr_ConnPtr);
    } else if ( deviceType.compare("smartcard")==0 ) {
        device = new SmartCardDevice(this);
    } else if ( deviceType.compare("input")==0 ) {
        device = new InputDevice(this);
    } else if ( deviceType.compare("hub")==0 ) {
        device = new HubDevice(this);
    } else if ( deviceType.compare("video")==0 ) {
        device = new VideoDevice(this);
    } else if ( deviceType.compare("sound")==0 ) {
        device = new SoundDevice(this);
    } else if ( deviceType.compare("hostdev")==0 ) {
        device = new HostDevice(
                    this,
                    ptr_ConnPtr);
    } else if ( deviceType.compare("graphics")==0 ) {
        device = new GraphicsDevice(
                    this,
                    ptr_ConnPtr);
    } else if ( deviceType.compare("redirdev")==0 ) {
        device = new RedirDevDevice(
                    this,
                    ptr_ConnPtr);
    } else if ( deviceType.compare("filesystem")==0 ) {
        device = new FileSystems(
                    this,
                    ptr_ConnPtr);
    } else if ( deviceType.compare("emulator")==0 ) {
        device = new _QWidget(this);
        //device = new Emulator(
        //            this,
        //            ptr_ConnPtr);
    } else if ( deviceType.compare("rng")==0 ) {
        device = new Random(this);
    } else if ( deviceType.compare("memballoon")==0 ) {
        device = new MemBalloon(
                    this,
                    ptr_ConnPtr);
    } else {
        device = new _QWidget(this);
    };
    connect(device, SIGNAL(errorMsg(const QString&)),
            this, SIGNAL(errorMsg(const QString&)));
    connect(device, SIGNAL(complete()),
            this, SLOT(deviceDataProcessed()));
    infoLayout->insertWidget(0, devIcon, -1, Qt::AlignHCenter);
    infoLayout->insertWidget(1, device, -1);
    infoLayout->insertStretch(-1);
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
