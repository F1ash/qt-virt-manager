#include "create_virt_domain.h"
#define TIMEOUT     60*1000
#define PERIOD      333

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
        QWidget *parent,
        virConnectPtr conn,
        QString _xmlFileName) :
    QDialog(parent), currWorkConnect(conn), xmlFileName(_xmlFileName)
{
    setModal(true);
    setWindowTitle("Domain Settings");
    restoreGeometry(settings.value("DomCreateGeometry").toByteArray());
    xml = new QTemporaryFile(this);
    xml->setAutoRemove(false);
    xml->setFileTemplate(
                QString("%1%2XML_Desc-XXXXXX.xml")
                .arg(QDir::tempPath())
                .arg(QDir::separator()));
    setEnabled(false);
    timerId = startTimer(PERIOD);
    readyDataLists();
}
CreateVirtDomain::~CreateVirtDomain()
{
    if ( timerId>0 ) killTimer(timerId);
    settings.setValue("DomCreateGeometry", saveGeometry());
    delete xml;
    xml = NULL;
    if ( ready ) {
        disconnect(ok, SIGNAL(clicked()), this, SLOT(set_Result()));
        disconnect(restore, SIGNAL(clicked()), this, SLOT(restoreParameters()));
        disconnect(cancel, SIGNAL(clicked()), this, SLOT(set_Result()));
        delete_specified_widgets();
        settings.setValue("DomCreateShowDesc", showDescription->isChecked());
        delete showDescription;
        showDescription = NULL;
        delete ok;
        ok = NULL;
        delete restore;
        restore = NULL;
        delete cancel;
        cancel = NULL;
        delete buttonLayout;
        buttonLayout = NULL;
        delete buttons;
        buttons = NULL;
        delete commonLayout;
        commonLayout = NULL;
    };
}

/* public slots */
QString CreateVirtDomain::getXMLDescFileName() const
{
    return xml->fileName();
}
bool CreateVirtDomain::getShowing() const
{
    return showDescription->isChecked();
}

/* private slots */
void CreateVirtDomain::readCapabilities()
{
    capabilities = QString("%1")
            .arg(virConnectGetCapabilities(currWorkConnect));
    //qDebug()<<capabilities;
    QDomDocument doc;
    doc.setContent(capabilities);
    QDomElement _domain = doc.
            firstChildElement("capabilities").
            firstChildElement("guest").
            firstChildElement("arch").
            firstChildElement("domain");
    if ( !_domain.isNull() ) {
        type = _domain.attribute("type");
    };
}
void CreateVirtDomain::readyDataLists()
{
    readCapabilities();
    if ( xmlFileName.isEmpty() ) {
        // create/define new VM
    } else {
        // read for edit exist VM parameters
        QTemporaryFile *_xml =
                new QTemporaryFile(this);
        _xml->setFileName(xmlFileName);
        _xml->setAutoRemove(true);
        _xml->open();
        xmlDesc.append(_xml->readAll().constData());
        _xml->close();
        delete _xml;
        _xml = NULL;
        //qDebug()<<xmlDesc<<"desc";
    };
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
            showDescription = new QCheckBox("Show XML Description\nat close", this);
            showDescription->setChecked(settings.value("DomCreateShowDesc").toBool());
            ok = new QPushButton(QIcon::fromTheme("dialog-ok"), "Ok", this);
            ok->setAutoDefault(true);
            connect(ok, SIGNAL(clicked()), this, SLOT(set_Result()));
            restore = new QPushButton(QIcon::fromTheme("go-first"), "Restore", this);
            restore->setToolTip("Restore all");
            connect(restore, SIGNAL(clicked()), this, SLOT(restoreParameters()));
            cancel = new QPushButton(QIcon::fromTheme("dialog-cancel"), "Cancel", this);
            cancel->setAutoDefault(true);
            connect(cancel, SIGNAL(clicked()), this, SLOT(set_Result()));
            buttonLayout = new QHBoxLayout();
            buttonLayout->addWidget(showDescription);
            buttonLayout->addWidget(ok);
            buttonLayout->addWidget(restore);
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
    this->setEnabled(false);
    QDomDocument doc;
    QDomElement root, devices, _element;
    root = doc.createElement("domain");
    root.setAttribute("type", type.toLower());
    doc.appendChild(root);
    devices = doc.createElement("devices");
    foreach (QString key, wdgList.keys()) {
        _QWidget *Wdg = static_cast<_QWidget*>(
                    wdgList.value(key));
        if ( NULL==Wdg ) continue;
        tabWidget->setCurrentWidget(Wdg);
        QDomNodeList list;
        if ( key=="Computer" ) {
            list = Wdg->getDataDocument().firstChildElement("devices").childNodes();
            _element = devices;
        } else {
            tabWidget->setCurrentWidget(Wdg);
            Wdg->closeDataEdit();
            list = Wdg->getDataDocument().firstChildElement("data").childNodes();
            _element = root;
        };
        /*
         * current DomNode is removed to root-element
         * but NULL-elemens not removed
         * therefore keep to seek on not-NULL next element
         */
        uint j = 0;
        uint count = list.length();
        for (uint i=0; i<count; i++) {
            //qDebug()<<list.item(j).nodeName()<<i;
            if (!list.item(j).isNull()) _element.appendChild(list.item(j));
            else ++j;
        };
    };
    root.appendChild(devices);
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
    if ( !type.isEmpty() ) {
        wdgList.insert("General", new General(this, capabilities, xmlDesc));
        wdgList.insert("OS_Booting", new OS_Booting(this, capabilities, xmlDesc));
        wdgList.insert("Memory", new Memory(this, capabilities, xmlDesc));
        wdgList.insert("CPU", new CPU(this, xmlDesc));
        wdgList.insert("Computer",
                       new Devices(this, currWorkConnect, xmlDesc));
        wdgList.insert("SecurityLabel", new SecurityLabel(this, xmlDesc));
        connect(wdgList.value("OS_Booting"), SIGNAL(domainType(QString&)),
                wdgList.value("General"), SLOT(changeArch(QString&)));
        connect(wdgList.value("OS_Booting"), SIGNAL(emulatorType(QString&)),
                wdgList.value("Computer"), SLOT(setEmulator(QString&)));
    } else wdgList.clear();
}
void CreateVirtDomain::set_specified_Tabs()
{
    if ( NULL==tabWidget ) tabWidget = new QTabWidget(this);
    foreach (QString key, wdgList.keys()) {
        uint idx;
        _QWidget *Wdg = static_cast<_QWidget*>(
                    wdgList.value(key));
        if ( NULL!=Wdg ) {
            if ( key=="General" ) idx = 0;
            else if ( key=="OS_Booting" ) idx = 1;
            else if ( key=="Memory" ) idx = 2;
            else if ( key=="CPU" ) idx = 3;
            else if ( key=="Computer" ) idx = 4;
            else if ( key=="SecurityLabel" ) idx = 5;
            else continue;
            tabWidget->insertTab(idx , Wdg, QIcon::fromTheme(key.toLower()), key);
        };
    };
    tabWidget->setCurrentIndex(0);
}
void CreateVirtDomain::delete_specified_widgets()
{
    tabWidget->clear();
    disconnect(wdgList.value("OS_Booting"), SIGNAL(domainType(QString&)),
               wdgList.value("General"), SLOT(changeArch(QString&)));
    disconnect(wdgList.value("OS_Booting"), SIGNAL(emulatorType(QString&)),
               wdgList.value("Computer"), SLOT(setEmulator(QString&)));
    foreach (QString key, wdgList.keys()) {
        _QWidget *Wdg = static_cast<_QWidget*>(
                    wdgList.value(key));
        if ( NULL!=Wdg ) {
            delete Wdg;
        };
    };
    wdgList.clear();
    commonLayout->removeWidget(tabWidget);
    delete tabWidget;
    tabWidget = NULL;
}
void CreateVirtDomain::restoreParameters()
{
    setEnabled(false);
    tabWidget->clear();
    disconnect(wdgList.value("OS_Booting"), SIGNAL(domainType(QString&)),
               wdgList.value("General"), SLOT(changeArch(QString&)));
    disconnect(wdgList.value("OS_Booting"), SIGNAL(emulatorType(QString&)),
               wdgList.value("Computer"), SLOT(setEmulator(QString&)));
    foreach (QString key, wdgList.keys()) {
        _QWidget *Wdg = static_cast<_QWidget*>(
                    wdgList.value(key));
        if ( NULL!=Wdg ) {
            delete Wdg;
        };
    };
    wdgList.clear();
    create_specified_widgets();
    set_specified_Tabs();
    setEnabled(true);
}
