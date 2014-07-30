#include "device_existance_menu.h"

DeviceExistanceMenu::DeviceExistanceMenu(QWidget *parent) :
    QMenu(parent)
{
    qRegisterMetaType<Device_Action>("Device_Action");
    addNewDevice = addAction(QIcon::fromTheme("list-add"), "Add Device");
    delSelDevice = addAction(QIcon::fromTheme("list-remove"), "Del Device");
    connect(addNewDevice, SIGNAL(triggered()), this, SLOT(emitJobSignal()));
    connect(delSelDevice, SIGNAL(triggered()), this, SLOT(emitJobSignal()));
}
DeviceExistanceMenu::~DeviceExistanceMenu()
{
    disconnect(addNewDevice, SIGNAL(triggered()), this, SLOT(emitJobSignal()));
    disconnect(delSelDevice, SIGNAL(triggered()), this, SLOT(emitJobSignal()));
    delete addNewDevice;
    addNewDevice = 0;
    delete delSelDevice;
    delSelDevice = 0;
}

/* public slots */

/* private slots */
void DeviceExistanceMenu::emitJobSignal()
{
    if ( sender()==addNewDevice ) {
        result = ADD;
    } else if ( sender()==delSelDevice ) {
        result = DEL;
    } else
        result = ERROR;
    emit resultSign(result);
}
