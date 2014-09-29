#include "hub_device_edit.h"

HubDevice_Edit::HubDevice_Edit(QWidget *parent) :
    HubDevice(parent)
{
    connect(addr, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}
