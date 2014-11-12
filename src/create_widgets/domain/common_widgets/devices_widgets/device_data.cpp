#include "device_data.h"

DeviceData::DeviceData(
        QWidget *parent,
        virConnectPtr conn
        ) :
    QWidget(parent), currWorkConnect(conn)
{
    devName = new QLabel(this);
    save = new QPushButton(QIcon::fromTheme("document-save"), "Save", this);
    save->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    save->setToolTip("Save current state");
    revert = new QPushButton(QIcon::fromTheme("document-revert"), "Revert", this);
    revert->setToolTip("Revert to previous state");
    revert->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    restoreMenu = new DeviceRestoreMenu(this);
    restoreMenu->revertData->setEnabled(false);
    revert->setMenu(restoreMenu);
    _close = new QPushButton(QIcon::fromTheme("window-close"), "Close", this);
    _close->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    panelLayout = new QHBoxLayout(this);
    panelLayout->addWidget(devName, 10, Qt::AlignLeft);
    panelLayout->addWidget(save, 1, Qt::AlignRight);
    panelLayout->addWidget(revert, 1, Qt::AlignRight);
    panelLayout->addWidget(_close, 1, Qt::AlignRight);
    panel = new QWidget(this);
    panel->setLayout(panelLayout);
    infoWidget = new QScrollArea(this);
    infoWidget->setWidgetResizable(true);
    scrolled = new QWidget(this);
    infoLayout = new QVBoxLayout(this);
    scrolled->setLayout(infoLayout);
    infoWidget->setWidget(scrolled);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(panel);
    commonLayout->addWidget(infoWidget);
    setLayout(commonLayout);
    connect(save, SIGNAL(clicked()),
            this, SLOT(saveDeviceData()));
    connect(restoreMenu->revertData, SIGNAL(triggered()),
            this, SLOT(revertDeviceData()));
    connect(restoreMenu->resetData, SIGNAL(triggered()),
            this, SLOT(revertDeviceData()));
    connect(_close, SIGNAL(clicked()),
            this, SLOT(closeDataEdit()));
}

/* public slots */
QDomDocument DeviceData::getResult() const
{
    //qDebug()<<"DeviceData result";
    QDomDocument doc;
    if ( device!=NULL ) {
        doc = device->getDataDocument();
    };
    return doc;
}
QString DeviceData::showDevice(QString &deviceName, QString &xmlDesc)
{
    QString _ret;
    if ( device!=NULL ) _ret = closeDataEdit();
    devName->setText(QString("<b>%1</b>").arg(deviceName));
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QString deviceType = doc.firstChildElement("device").
            firstChild().nodeName();
    if ( deviceType == "disk" ) {
        device = new Disk_Edit(
                    this,
                    currWorkConnect);
    } else if ( deviceType == "interface" ) {
        device = new NetInterfaces_Edit(
                    this,
                    currWorkConnect);
    } else if ( deviceType == "serial" ) {
        device = new CharDevice_Edit(this, NULL, NULL, deviceType);
    } else if ( deviceType == "parallel" ) {
        device = new CharDevice_Edit(this, NULL, NULL, deviceType);
    } else if ( deviceType == "channel" ) {
        device = new ChannelDevice(this);
    } else if ( deviceType == "console" ) {
        device = new ConsoleDevice_Edit(
                          this,
                          currWorkConnect);
    } else if ( deviceType == "smartcard" ) {
        device = new SmartCardDevice_Edit(this);
    } else if ( deviceType == "input" ) {
        device = new InputDevice_Edit(this);
    } else if ( deviceType == "hub" ) {
        device = new HubDevice_Edit(this);
    } else if ( deviceType == "video" ) {
        device = new VideoDevice_Edit(this);
    } else if ( deviceType == "sound" ) {
        device = new SoundDevice_Edit(this);
    } else if ( deviceType == "hostdev" ) {
        device = new HostDevice_Edit(
                          this,
                          currWorkConnect);
    } else if ( deviceType == "graphics" ) {
        device = new GraphicsDevice_Edit(
                    this,
                    currWorkConnect);
    } else if ( deviceType == "redirdev" ) {
        device = new RedirDevDevice_Edit(
                    this,
                    currWorkConnect);
    } else if ( deviceType == "filesystem" ) {
        device = new FileSystems_Edit(
                    this,
                    currWorkConnect);
    } else if ( deviceType == "emulator" ) {
        device = new Emulator_Edit(this);
    } else if ( deviceType == "rng" ) {
        device = new Random_Edit(this);
    } else if ( deviceType == "memballoon" ) {
        device = new MemBalloon_Edit(
                    this,
                    currWorkConnect);
    } else {
        device = new _QWidget(this);
    };
    infoLayout->insertWidget(0, device, -1);
    DeviceXMLDesc = xmlDesc;
    currentDeviceXMLDesc = xmlDesc;
    device->setDataDescription(xmlDesc);
    connect(device, SIGNAL(dataChanged()),
            this, SLOT(currentStateChanged()));
    return _ret;
}
QString DeviceData::closeDataEdit()
{
    QString _ret;
    if ( !currentStateSaved ) {
        int answer = QMessageBox::question(
                    this,
                    "Save Device Data",
                    "Save last changes?",
                    QMessageBox::Ok,
                    QMessageBox::Cancel);
        if ( answer==QMessageBox::Ok )
            saveDeviceData();
    };
    if ( NULL!=device ) {
        // save device data as null-point
        if ( sender()==_close )
            // quired by current open DeviceData widget
            emit saveDeviceXMLDesc(currentDeviceXMLDesc);
        else
            // quired by new open DeviceData widget
            _ret = currentDeviceXMLDesc;
    };
    if ( NULL!=device ) {
        infoLayout->removeWidget(device);
        disconnect(device, SIGNAL(dataChanged()),
                   this, SLOT(currentStateChanged()));
        delete device;
        device = NULL;
    };
    setStartState();
    return _ret;
}

/* private slots */
void DeviceData::currentStateChanged()
{
    currentStateSaved = false;
    restoreMenu->revertData->setEnabled(true);
}
void DeviceData::readNetworkList()
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
void DeviceData::readNodeDevicesList()
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
void DeviceData::saveDeviceData()
{
    // save device data as previous state
    if ( NULL!=device ) {
        currentDeviceXMLDesc = device->getDataDocument().toString();
    };
    currentStateSaved = true;
    restoreMenu->revertData->setEnabled(false);
}
void DeviceData::revertDeviceData()
{
    if ( NULL==device ) return;
    QAction *act = static_cast<QAction*>(sender());
    if ( act==restoreMenu->revertData ) {
        // revert device data from previous state
        device->setDataDescription(currentDeviceXMLDesc);
    } else if ( act==restoreMenu->resetData ) {
        // restore device data from null-point
        currentDeviceXMLDesc.clear();
        device->setDataDescription(DeviceXMLDesc);
        currentDeviceXMLDesc = DeviceXMLDesc;
    };
    currentStateSaved = true;
    restoreMenu->revertData->setEnabled(false);
}
void DeviceData::setStartState()
{
    devName->clear();
    DeviceXMLDesc.clear();
    currentDeviceXMLDesc.clear();
    currentStateSaved = true;
    restoreMenu->revertData->setEnabled(false);
}

/* unused without
 * readNetworkList() & void readNodeDevicesList()
 */
void DeviceData::sendConnErrors()
{
    virtErrors = virConnGetLastError(currWorkConnect);
    if ( virtErrors!=NULL && virtErrors->code>0 ) {
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                       .arg(QString().fromUtf8(virtErrors->message)) );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void DeviceData::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL && virtErrors->code>0 )
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                       .arg(QString().fromUtf8(virtErrors->message)) );
    virResetLastError();
}
