#include "device_data.h"

DeviceData::DeviceData(
        QWidget *parent, virConnectPtr *conn ) :
    QWidget(parent), ptr_ConnPtr(conn)
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
void DeviceData::showDevice(int idx, QString &deviceName, QString &xmlDesc)
{
    if ( device!=NULL ) closeDataEdit();
    devName->setText(QString("<b>%1</b>").arg(deviceName));
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QString deviceType = doc.firstChildElement("device").
            firstChild().nodeName();
    if ( deviceType == "disk" ) {
        device = new Disk_Edit(
                    this,
                    ptr_ConnPtr);
    } else if ( deviceType == "interface" ) {
        device = new NetInterfaces_Edit(
                    this,
                    ptr_ConnPtr);
    } else if ( deviceType == "serial" ) {
        device = new CharDevice_Edit(this, NULL, NULL, deviceType);
    } else if ( deviceType == "parallel" ) {
        device = new CharDevice_Edit(this, NULL, NULL, deviceType);
    } else if ( deviceType == "channel" ) {
        device = new ChannelDevice(this);
    } else if ( deviceType == "console" ) {
        device = new ConsoleDevice_Edit(
                          this,
                          ptr_ConnPtr);
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
                          ptr_ConnPtr);
    } else if ( deviceType == "graphics" ) {
        device = new GraphicsDevice_Edit(
                    this,
                    ptr_ConnPtr);
    } else if ( deviceType == "redirdev" ) {
        device = new RedirDevDevice_Edit(
                    this,
                    ptr_ConnPtr);
    } else if ( deviceType == "filesystem" ) {
        device = new FileSystems_Edit(
                    this,
                    ptr_ConnPtr);
    } else if ( deviceType == "emulator" ) {
        device = new Emulator_Edit(this);
    } else if ( deviceType == "rng" ) {
        device = new Random_Edit(this);
    } else if ( deviceType == "memballoon" ) {
        device = new MemBalloon_Edit(
                    this,
                    ptr_ConnPtr);
    } else {
        device = new _QWidget(this);
    };
    infoLayout->insertWidget(0, device, -1);
    DeviceXMLDesc = xmlDesc;
    currentDeviceXMLDesc = xmlDesc;
    currentItemRow = idx;
    device->setDataDescription(xmlDesc);
    connect(device, SIGNAL(dataChanged()),
            this, SLOT(currentStateChanged()));
}
void DeviceData::closeDataEdit()
{
    if ( !currentStateSaved ) {
        int answer = QMessageBox::question(
                    this,
                    "Save Device Data",
                    "In Devices:\nSave last changes?",
                    QMessageBox::Ok,
                    QMessageBox::Cancel);
        if ( answer==QMessageBox::Ok ) {
            saveDeviceData();
        } else
            setStartState();
    };
    if ( NULL!=device && changed ) {
        QString xmlDesc = currentDeviceXMLDesc;
        int row = currentItemRow;
        setStartState();
        emit saveDeviceXMLDesc(row, xmlDesc);
        //qDebug()<<"emited"<<devName->text();
    };
    if ( NULL!=device ) {
        infoLayout->removeWidget(device);
        disconnect(device, SIGNAL(dataChanged()),
                   this, SLOT(currentStateChanged()));
        delete device;
        device = NULL;
    };
}

/* private slots */
void DeviceData::currentStateChanged()
{
    currentStateSaved = false;
    restoreMenu->revertData->setEnabled(true);
    changed = true;
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
    currentItemRow = -1;
    currentStateSaved = true;
    restoreMenu->revertData->setEnabled(false);
    changed = false;
}
