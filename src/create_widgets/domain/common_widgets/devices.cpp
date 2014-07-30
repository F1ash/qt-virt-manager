#include "devices.h"

Devices::Devices(QWidget *parent) :
    _QWidget(parent)
{
    setObjectName("Computer");
    usedDeviceList = new QListWidget(this);
    usedDeviceList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(usedDeviceList,
            SIGNAL(customContextMenuRequested(const QPoint&)),
            this,
            SLOT(showContextMenu(const QPoint&)));
    addNewDevice = new QPushButton(QIcon::fromTheme("list-add"), "Add Device", this);
    connect(addNewDevice, SIGNAL(clicked()), this, SLOT(choiceNewDevice()));

    listLayout = new QVBoxLayout(this);
    listLayout->addWidget(usedDeviceList);
    listLayout->addWidget(addNewDevice);
    listWidget = new QWidget(this);
    listWidget->setLayout(listLayout);

    infoWidget = new QWidget(this);
    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(listWidget, 3);
    commonLayout->addWidget(infoWidget, 8);
    detectAttachedDevicesFromXMLDesc();
}
Devices::~Devices()
{
    disconnect(usedDeviceList,
               SIGNAL(customContextMenuRequested(const QPoint&)),
               this,
               SLOT(showContextMenu(const QPoint&)));
    disconnect(addNewDevice, SIGNAL(clicked()), this, SLOT(choiceNewDevice()));
    delete usedDeviceList;
    usedDeviceList = 0;
    delete addNewDevice;
    addNewDevice = 0;

    delete listLayout;
    listLayout = 0;
    delete listWidget;
    listWidget = 0;

    delete infoWidget;
    infoWidget = 0;
    delete commonLayout;
    commonLayout = 0;
}

/* public slots */

/* private slots */
void Devices::choiceNewDevice()
{
    qDebug()<<"add New Device";
}
void Devices::showContextMenu(const QPoint &pos)
{
    usedDeviceList->setCurrentItem(usedDeviceList->itemAt(pos));
    DeviceExistanceMenu *jobMenu = new DeviceExistanceMenu(this);
    connect(jobMenu, SIGNAL(resultSign(Device_Action)),
            this, SLOT(execDevExistanceMenuResult(Device_Action)));
    jobMenu->move(mapToGlobal(pos));
    jobMenu->exec();
    disconnect(jobMenu, SIGNAL(resultSign(Device_Action)),
               this, SLOT(execDevExistanceMenuResult(Device_Action)));
    jobMenu->deleteLater();
}
void Devices::execDevExistanceMenuResult(Device_Action ret)
{
    if ( ret==ADD ) {
        choiceNewDevice();
    } else if ( ret==DEL ) {
        qDebug()<<"Delete"<<usedDeviceList->currentItem()->text();
    };
}
void Devices::detectAttachedDevicesFromXMLDesc()
{
    usedDeviceList->addItem("Network");
}
