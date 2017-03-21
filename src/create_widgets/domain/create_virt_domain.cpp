#include "create_virt_domain.h"
#include "common_widgets/general.h"
#include "common_widgets/os_booting.h"
#include "common_widgets/memory.h"
#include "common_widgets/cpu.h"
#include "common_widgets/security_label.h"
#include "common_widgets/misc_settings.h"
#include "common_widgets/devices.h"
#include <QTime>

HelperThread::HelperThread(QObject *parent, virConnectPtr *connPtrPtr) :
    _VirtThread(parent, connPtrPtr)
{

}
void HelperThread::run()
{
    if ( nullptr==ptr_ConnPtr || nullptr==*ptr_ConnPtr ) {
        emit ptrIsNull();
        return;
    };
    if ( virConnectRef(*ptr_ConnPtr)<0 ) {
        sendConnErrors();
        return;
    };
    capabilities = QString(
                virConnectGetCapabilities(*ptr_ConnPtr));
    if ( capabilities.isEmpty() ) sendConnErrors();
    virNodeInfo info;
    int res = virNodeGetInfo(*ptr_ConnPtr, &info);
    if ( res==0 ) {
        cores = info.cores * info.sockets;
    };
    if ( virConnectClose(*ptr_ConnPtr)<0 )
        sendConnErrors();
}

/*
 * http://libvirt.org/formatdomain.html
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

CreateVirtDomain::CreateVirtDomain(
        QWidget *parent, TASK _task) :
    QMainWindow(parent), task(_task)
{
    setAttribute(Qt::WA_DeleteOnClose);
    xmlFileName = task.args.path;
    ptr_ConnPtr = task.srcConnPtr;
    setWindowTitle("VM Settings");
    setWindowIcon(QIcon::fromTheme("virtual-engineering"));
    restoreGeometry(settings.value("DomCreateGeometry").toByteArray());
    xml = new QTemporaryFile(this);
    xml->setAutoRemove(false);
    xml->setFileTemplate(
                QString("%1%2XML_Desc-XXXXXX.xml")
                .arg(QDir::tempPath())
                .arg(QDir::separator()));
    setEnabled(false);
    hlpThread = new HelperThread(this, ptr_ConnPtr);
    connect(hlpThread, SIGNAL(finished()),
            this, SLOT(readCapabilities()));
    connect(hlpThread, SIGNAL(errorMsg(const QString&, const uint)),
            this, SIGNAL(errorMsg(const QString&)));
    hlpThread->start();
}
CreateVirtDomain::~CreateVirtDomain()
{
    settings.setValue("DomCreateGeometry", saveGeometry());
    if ( ready ) {
        //settings.setValue("DomCreateShowDesc", showDescription->isChecked());
    };
}

/* private slots */
void CreateVirtDomain::closeEvent(QCloseEvent *ev)
{
    if ( ev->type()==QEvent::Close ) {
        QString key = objectName();
        QString msg = QString("'<b>%1</b>' domain editor closed.")
                .arg(task.object);
        sendMsg(msg);
        emit finished(key);
    };
}
void CreateVirtDomain::readCapabilities()
{
    //qDebug()<<hlpThread->capabilities;
    QDomDocument doc;
    doc.setContent(hlpThread->capabilities);
    QDomElement _domain = doc.
            firstChildElement("capabilities").
            firstChildElement("guest").
            firstChildElement("arch").
            firstChildElement("domain");
    if ( !_domain.isNull() ) {
        type = _domain.attribute("type");
        ready = true;
    } else
        ready = false;
    if ( xmlFileName.isEmpty() ) {
        // create/define new VM
        if (type.toLower()=="lxc") {
            QFile _xml(":/misc/domain/lxc_example1.xml", this);
            _xml.open(QIODevice::ReadOnly);
            xmlDesc.append(_xml.readAll().constData());
            _xml.close();
        } else if ( type.toLower()=="qemu" ) {
            QFile _xml(":/misc/domain/qemu_guest_x86_64.xml", this);
            _xml.open(QIODevice::ReadOnly);
            xmlDesc.append(_xml.readAll().constData());
            _xml.close();
        }
    } else {
        // read for edit exist VM parameters
        QFile *_xml = new QFile(this);
        _xml->setFileName(xmlFileName);
        _xml->open(QIODevice::ReadOnly);
        xmlDesc.append(_xml->readAll().constData());
        _xml->close();
        _xml->deleteLater();
    };
    //qDebug()<<xmlDesc<<"desc"<<type<<xmlFileName;
    readDataLists();
}
void CreateVirtDomain::readDataLists()
{
    if ( ready ) {
        create_specified_widgets();
        set_specified_Tabs();
        about = new QLabel(
        "<a href='http://libvirt.org/formatdomain.html'>About</a>",
                    this);
        about->setToolTip("http://libvirt.org/formatdomain.html");
        about->setOpenExternalLinks(true);
        //showDescription = new QCheckBox("Show XML Description\nat close", this);
        //showDescription->setChecked(settings.value("DomCreateShowDesc").toBool());
        ok = new QPushButton(
                    QIcon::fromTheme("dialog-ok"),
                    "Ok",
                    this);
        ok->setAutoDefault(true);
        connect(ok, SIGNAL(clicked()), this, SLOT(set_Result()));
        restore = new QPushButton(
                    QIcon::fromTheme("go-first"),
                    "Restore all",
                    this);
        restore->setToolTip("Restore all pages to first state");
        connect(restore, SIGNAL(clicked()),
                this, SLOT(restoreParameters()));
        cancel = new QPushButton(
                    QIcon::fromTheme("dialog-cancel"),
                    "Cancel",
                    this);
        cancel->setAutoDefault(true);
        connect(cancel, SIGNAL(clicked()),
                this, SLOT(set_Result()));
        buttonLayout = new QHBoxLayout();
        buttonLayout->addWidget(about);
        //buttonLayout->addWidget(showDescription);
        buttonLayout->addWidget(ok);
        buttonLayout->addWidget(restore);
        buttonLayout->addWidget(cancel);
        buttons = new QWidget(this);
        buttons->setLayout(buttonLayout);
        commonLayout = new QVBoxLayout();
        commonLayout->addWidget(tabWidget);
        commonLayout->addWidget(buttons);
        baseWdg = new QWidget(this);
        baseWdg->setLayout(commonLayout);
        setEnabled(true);
        setCentralWidget(baseWdg);
    } else {
        QString msg = QString("Read Data in %1 failed.")
                .arg(objectName());
        sendMsg( msg );
        // to done()
        set_Result();
    };
}
bool CreateVirtDomain::buildXMLDescription()
{
    /*
     * build Device description
     * then common description append it
     */
    this->setEnabled(false);
    QDomDocument doc;
    QDomElement root, devices, _element, _el;
    root = doc.createElement("domain");
    root.setAttribute("type", type.toLower());
    doc.appendChild(root);
    devices = doc.createElement("devices");
    foreach (QString key, wdgList.keys()) {
        _QWidget *Wdg = static_cast<_QWidget*>(wdgList.value(key));
        if ( nullptr==Wdg ) continue;
        tabWidget->setCurrentWidget(Wdg);
        QDomNodeList list;
        if ( key=="Devices" ) {
            tabWidget->setCurrentWidget(Wdg);
            bool ret = Wdg->closeDataEdit();
            if ( !ret ) return false;
            _el = Wdg->getDataDocument().firstChildElement("devices");
            setBootOrder(&_el);
            if ( !_el.isNull() ) list = _el.childNodes();
            _element = devices;
        } else {
            tabWidget->setCurrentWidget(Wdg);
            bool ret = Wdg->closeDataEdit();
            if ( !ret ) return false;
            _el = Wdg->getDataDocument().firstChildElement("data");
            if ( !_el.isNull() ) list = _el.childNodes();
            _element = root;
        };
        /*
         * current DomNode is removed to root-element
         * but nullptr-elemens not removed
         * therefore keep to seek on not-nullptr next element
         */
        uint j = 0;
        int count = list.length();
        for (int i=0; i<count; i++) {
            //qDebug()<<list.item(j).nodeName()<<i;
            if (!list.item(j).isNull())
                _element.appendChild(list.item(j));
            else ++j;
        };
    };
    root.appendChild(devices);
    //qDebug()<<doc.toByteArray(4).data();

    bool read = xml->open();
    if (read) xml->write(doc.toByteArray(4).data());
    xml->close();
    return true;
}
void CreateVirtDomain::set_Result()
{
    if ( sender()==ok ) {
        if ( !buildXMLDescription() ) {
            this->setEnabled(true);
            return;
        };
        QString _xml = xml->fileName();
        QStringList data;
        data.append("New Domain XML'ed");
        data.append(QString("to <a href='%1'>%1</a>").arg(_xml));
        QString msg = data.join(" ");
        sendMsg(msg);
        // if ( showDescription->isChecked() )
        //     QDesktopServices::openUrl(QUrl(_xml));
        task.args.path = _xml;
        emit addNewTask(&task);
    };
    close();
}
void CreateVirtDomain::create_specified_widgets()
{
    if ( !type.isEmpty() ) {
        wdgList.insert(
                    "General",
                    new General(this, hlpThread->capabilities, xmlDesc));
        wdgList.insert(
                    "Misc.",
                    new Misc_Settings(this, hlpThread->capabilities, xmlDesc));
        wdgList.insert(
                    "OS_Booting",
                    new OS_Booting(this, hlpThread->capabilities, xmlDesc));
        wdgList.insert(
                    "Memory",
                    new Memory(this, hlpThread->capabilities, xmlDesc));
        wdgList.insert(
                    "CPU",
                    new CPU(this, hlpThread->capabilities, xmlDesc, hlpThread->cores));
        wdgList.insert(
                    "Devices",
                    new Devices(this, ptr_ConnPtr, xmlDesc));
        wdgList.insert(
                    "SecurityLabel",
                    new SecurityLabel(this, xmlDesc));
        connect(wdgList.value("General"), SIGNAL(newName(const QString&)),
                this, SLOT(setNewWindowTitle(const QString&)));
        connect(wdgList.value("OS_Booting"), SIGNAL(domainType(const QString&)),
                wdgList.value("General"), SLOT(changeArch(const QString&)));
        connect(wdgList.value("OS_Booting"), SIGNAL(emulatorType(const QString&)),
                wdgList.value("Devices"), SLOT(setEmulator(const QString&)));
        connect(wdgList.value("Devices"), SIGNAL(devicesChanged(QDomDocument&)),
                wdgList.value("OS_Booting"), SLOT(searchBootableDevices(QDomDocument&)));
        connect(wdgList.value("OS_Booting"), SIGNAL(maxVCPU(const QString&)),
                wdgList.value("CPU"), SLOT(setMaxVCPU(const QString&)));
        connect(wdgList.value("OS_Booting"), SIGNAL(archChanged(const QString&)),
                wdgList.value("CPU"), SLOT(changeArch(const QString&)));
    } else {
        wdgList.clear();
        QMessageBox::information(
                    this,
                    "Info",
                    "Type of VM not defined.\nPlease, install nessesary drivers.",
                    QMessageBox::Ok);
    };
}
void CreateVirtDomain::set_specified_Tabs()
{
    if ( nullptr==tabWidget ) tabWidget = new QTabWidget(this);
    for (int idx=0; idx<wdgList.count(); idx++ ) {
        QString key;
        switch (idx) {
        case 0:
            key = "General";
            break;
        case 1:
            key = "Misc.";
            break;
        case 2:
            key = "OS_Booting";
            break;
        case 3:
            key = "CPU";
            break;
        case 4:
            key = "Memory";
            break;
        case 5:
            key = "Devices";
            break;
        case 6:
            key = "SecurityLabel";
            break;
        default:
            break;
        };
        _QWidget *Wdg = static_cast<_QWidget*>(wdgList.value(key));
        if ( nullptr!=Wdg ) {
            if ( idx == 2 ) {
                OS_Booting *wdg = static_cast<OS_Booting*>(Wdg);
                if ( nullptr!=wdg ) wdg->initMaxVCPU();
            } else if ( idx == 5 ) {
                Devices *wdg = static_cast<Devices*>(Wdg);
                if ( nullptr!=wdg ) {
                    connect(wdg, SIGNAL(errorMsg(const QString&)),
                            this, SIGNAL(errorMsg(const QString&)));
                    wdg->initBootDevices();
                };
            };
            tabWidget->insertTab(
                        idx,
                        Wdg,
                        QIcon::fromTheme(
                            Wdg->objectName().toLower()),
                        key);
        };
    };
    tabWidget->setCurrentIndex(0);
}
void CreateVirtDomain::restoreParameters()
{
    setEnabled(false);
    tabWidget->clear();
    //disconnect(wdgList.value("OS_Booting"), SIGNAL(domainType(const QString&)),
    //           wdgList.value("General"), SLOT(changeArch(const QString&)));
    //disconnect(wdgList.value("OS_Booting"), SIGNAL(emulatorType(const QString&)),
    //           wdgList.value("Devices"), SLOT(setEmulator(QString&)));
    foreach (QString key, wdgList.keys()) {
        _QWidget *Wdg = static_cast<_QWidget*>(wdgList.value(key));
        if ( nullptr!=Wdg ) {
            Wdg->deleteLater();
        };
    };
    wdgList.clear();
    create_specified_widgets();
    set_specified_Tabs();
    setEnabled(true);
}
void CreateVirtDomain::setBootOrder(QDomElement *_devices)
{
    OS_Booting *Wdg = static_cast<OS_Booting*>(wdgList.value("OS_Booting"));
    if ( nullptr!=Wdg ) {
        BootOrderList list = Wdg->getBootOrder();
        QDomDocument _bootDevDoc;
        _bootDevDoc.setContent(QString());
        foreach (BootOrder _data, list) {
            QDomDocument _doc;
            _doc.setContent(_data.deviceDesc);
            QDomElement _dev = _doc.firstChildElement();
            QDomElement _taggedDev = _devices->firstChildElement(_dev.tagName());
            while ( !_taggedDev.isNull() ) {
                QDomElement _boot = _taggedDev.firstChildElement("boot");
                if ( !_boot.isNull() ) {
                    _taggedDev.removeChild(_boot);
                };
                QDomNode _el = _devices->removeChild(_taggedDev);
                _bootDevDoc.appendChild(_el);
                _taggedDev = _devices->firstChildElement(_dev.tagName());
            };
        };
        //qDebug()<<_bootDevDoc.toString()<<"booted";
        QDomNodeList _list = _bootDevDoc.childNodes();
        int count = _list.count();
        for(int i=0; i<count; i++) {
            QDomNode _dev = _list.at(0);
            QDomDocument _doc1;
            _doc1.setContent(QString());
            _doc1.appendChild(_dev);
            foreach (BootOrder _data, list) {
                //qDebug()<<_data.deviceDesc<<_data.usage<<_data.order;
                QDomDocument _doc;
                _doc.setContent(_data.deviceDesc);
                QDomElement _new_dev = _doc.firstChildElement();
                if ( !_new_dev.firstChildElement("boot").isNull() ) {
                    _new_dev.removeChild(_new_dev.firstChildElement("boot"));
                };
                //qDebug()<<_doc.toString()<<_doc1.toString();
                if ( _doc.toString()==_doc1.toString() ) {
                    if ( _data.usage ) {
                        QDomElement _boot = _doc.createElement("boot");
                        _boot.setAttribute("order", _data.order);
                        _dev.appendChild(_boot);
                        //qDebug()<<_doc.toString()<<_data.order;
                    };
                };
            };
            _devices->appendChild(_doc1.firstChildElement());
        };
    };
}
void CreateVirtDomain::setNewWindowTitle(const QString &_name)
{
    QString connName = task.srcConName;
    setWindowTitle(
                QString("VM Settings / <%1> in [%2]")
                .arg(_name).arg(connName));
}
void CreateVirtDomain::sendMsg(const QString &msg)
{
    QString time = QTime::currentTime().toString();
    QString title = QString("Connection '%1'").arg(task.srcConName);
    QString currMsg = QString(
    "<b>%1 %2:</b><br><font color='blue'><b>EVENT</b></font>: %3")
            .arg(time).arg(title).arg(msg);
    emit errorMsg(currMsg);
}
