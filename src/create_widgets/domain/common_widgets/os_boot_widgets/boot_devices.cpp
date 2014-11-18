#include "boot_devices.h"

Boot_Devices::Boot_Devices(QWidget *parent) :
    QWidget(parent)
{
    up = new QPushButton(QIcon::fromTheme("go-up"), "", this);
    down = new QPushButton(QIcon::fromTheme("go-down"), "", this);
    devices = new QListWidget(this);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(devices, 0, 1, 2, 1);
    commonLayout->addWidget(up, 0, 2);
    commonLayout->addWidget(down, 1, 2);
    setLayout(commonLayout);
    connect(up, SIGNAL(clicked()), this, SLOT(itemUp()));
    connect(down, SIGNAL(clicked()), this, SLOT(itemDown()));
    // dataChanged connections
    connect(devices, SIGNAL(itemChanged(QListWidgetItem*)),
            this, SIGNAL(dataChanged()));
    connect(devices, SIGNAL(currentRowChanged(int)),
            this, SIGNAL(dataChanged()));
}

/* public slots */
void Boot_Devices::addNewDevice(QString &_devName, bool _used, int _order)
{
    //qDebug()<<_devName;
    QListWidgetItem *_item = new QListWidgetItem();
    _item->setText(_devName);
    _item->setCheckState( (_used)? Qt::Checked:Qt::Unchecked);
    _item->setIcon(QIcon::fromTheme("computer"));
    devices->insertItem(_order, _item);
}

/* public slots */
void Boot_Devices::itemUp()
{
    int idx = devices->row( devices->currentItem() );
    if ( idx>=0 ) {
        QListWidgetItem *_item = devices->takeItem(idx);
        devices->insertItem(idx-1, _item);
        devices->setCurrentItem(_item);
    };
}
void Boot_Devices::itemDown()
{
    int idx = devices->row( devices->currentItem() );
    if ( idx>=0 ) {
        QListWidgetItem *_item = devices->takeItem(idx);
        devices->insertItem(idx+1, _item);
        devices->setCurrentItem(_item);
    };
}
