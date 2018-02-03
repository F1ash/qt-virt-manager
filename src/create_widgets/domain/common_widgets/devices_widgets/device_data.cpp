#include "device_data.h"
#include "create_widgets/domain/edit_widgets/emulator_edit.h"
#include "create_widgets/domain/edit_widgets/hub_device_edit.h"
#include "create_widgets/domain/edit_widgets/smartcard_device_edit.h"
#include "create_widgets/domain/edit_widgets/char_device_edit.h"
#include "create_widgets/domain/edit_widgets/sound_device_edit.h"
#include "create_widgets/domain/edit_widgets/console_device_edit.h"
#include "create_widgets/domain/edit_widgets/input_device_edit.h"
#include "create_widgets/domain/edit_widgets/memballoon_edit.h"
#include "create_widgets/domain/edit_widgets/redirdev_device_edit.h"
#include "create_widgets/domain/edit_widgets/video_device_edit.h"
#include "create_widgets/domain/edit_widgets/random_edit.h"
#include "create_widgets/domain/edit_widgets/host_device_edit.h"
#include "create_widgets/domain/edit_widgets/graphics_device_edit.h"
#include "create_widgets/domain/edit_widgets/net_interfaces_edit.h"
#include "create_widgets/domain/edit_widgets/filesystems_edit.h"
#include "create_widgets/domain/edit_widgets/disk_edit.h"

DeviceData::DeviceData(
        QWidget *parent, virConnectPtr *conn ) :
    QWidget(parent), ptr_ConnPtr(conn)
{
    devName = new QLabel(this);
    save = new QPushButton(
                QIcon::fromTheme("document-save"), "", this);
    save->setSizePolicy(
                QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    save->setToolTip(tr("Save current device state"));
    revert = new QPushButton(
                QIcon::fromTheme("document-revert"), "", this);
    revert->setToolTip(
                tr("Revert to previous device state"));
    revert->setSizePolicy(
                QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
    restoreMenu = new DeviceRestoreMenu(this);
    restoreMenu->revertData->setEnabled(false);
    revert->setMenu(restoreMenu);
    _close = new QPushButton(
                QIcon::fromTheme("dialog-close"), "", this);
    _close->setToolTip(tr("Close device editor"));
    _close->setSizePolicy(
                QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
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
    if ( device!=nullptr ) {
        doc = device->getDataDocument();
    };
    return doc;
}
void DeviceData::showDevice(
        const int idx, const QString &deviceName, const QString &xmlDesc)
{
    if ( device!=nullptr ) closeDataEdit();
    emit dataProcessed(false);
    devName->setText(QString("<b>%1</b>").arg(deviceName));
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QString deviceType;
    QDomElement _el = doc.firstChildElement("device");
    if ( _el.isNull() ) return;
    QDomNode _node = _el.firstChild();
    if ( _node.isNull() ) return;
    deviceType = _node.nodeName();
    devIcon = new QLabel(this);
    devIcon->setPixmap(QIcon::fromTheme(
                           QString("device-%1").arg(deviceType))
                       .pixmap(64));
    if        ( deviceType == "disk" ) {
        device = new Disk_Edit(
                    this,
                    ptr_ConnPtr);
    } else if ( deviceType == "interface" ) {
        device = new NetInterfaces_Edit(
                    this,
                    ptr_ConnPtr);
    } else if ( deviceType == "serial" ) {
        device = new CharDevice_Edit(
                    this,
                    nullptr,
                    nullptr,
                    deviceType);
    } else if ( deviceType == "parallel" ) {
        device = new CharDevice_Edit(
                    this,
                    nullptr,
                    nullptr,
                    deviceType);
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
    connect(device, SIGNAL(errorMsg(const QString&)),
            this, SIGNAL(errorMsg(const QString&)));
    connect(device, SIGNAL(complete()),
            this, SLOT(deviceDataProcessed()));
    infoLayout->insertWidget(0, devIcon, -1, Qt::AlignHCenter);
    infoLayout->insertWidget(1, device, -1);
    infoLayout->insertStretch(-1);
    DeviceXMLDesc = xmlDesc;
    currentDeviceXMLDesc = xmlDesc;
    currentItemRow = idx;
}
void DeviceData::closeDataEdit()
{
    if ( !currentStateSaved ) {
        int answer = QMessageBox::question(
                    this,
                    tr("Save Device Data"),
                    tr("In Devices:\nSave last changes?"),
                    QMessageBox::Ok,
                    QMessageBox::Cancel);
        if ( answer==QMessageBox::Ok ) {
            saveDeviceData();
        } else
            setStartState();
    };
    if ( nullptr!=device && changed ) {
        QString xmlDesc = currentDeviceXMLDesc;
        int row = currentItemRow;
        setStartState();
        emit saveDeviceXMLDesc(row, xmlDesc);
        //qDebug()<<"emited"<<devName->text();
    };
    clearDataEdit();
}
void DeviceData::clearDataEdit()
{
    if ( nullptr!=device ) {
        infoLayout->removeWidget(device);
        delete device;
        device = nullptr;
    };
    if ( devIcon!=nullptr ) {
        infoLayout->removeWidget(devIcon);
        delete devIcon;
        devIcon = nullptr;
    };
    deviceDataProcessed();
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
    if ( nullptr!=device ) {
        currentDeviceXMLDesc = device->getDataDocument().toString();
    };
    currentStateSaved = true;
    restoreMenu->revertData->setEnabled(false);
}
void DeviceData::revertDeviceData()
{
    if ( nullptr==device ) return;
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
void DeviceData::deviceDataProcessed()
{
    if ( nullptr!=device ) {
        device->setDataDescription(currentDeviceXMLDesc);
        connect(device, SIGNAL(dataChanged()),
                this, SLOT(currentStateChanged()));
    };
    emit dataProcessed(true);
}
