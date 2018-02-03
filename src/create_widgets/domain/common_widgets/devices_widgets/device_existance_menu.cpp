#include "device_existance_menu.h"

DeviceExistanceMenu::DeviceExistanceMenu(
        QWidget *parent, bool valid) :
    QMenu(parent), validItem(valid)
{
    qRegisterMetaType<Device_Action>("Device_Action");
    addNewDevice = addAction(
                QIcon::fromTheme("list-add"),
                tr("Add Device"));
    connect(addNewDevice, SIGNAL(triggered()),
            this, SLOT(emitJobSignal()));
    if (validItem) {
        delSelDevice = addAction(
                    QIcon::fromTheme("list-remove"),
                    tr("Del Device"));
        connect(delSelDevice, SIGNAL(triggered()),
                this, SLOT(emitJobSignal()));
    };
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
