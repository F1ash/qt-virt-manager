#ifndef VIRT_INTERFACE_CONTROL_H
#define VIRT_INTERFACE_CONTROL_H

#include "virt_objects/virt_entity_control.h"

class VirtInterfaceControl : public VirtEntityControl
{
    Q_OBJECT
public:
    explicit VirtInterfaceControl(QWidget *parent = NULL);
};

#endif // VIRT_INTERFACE_CONTROL_H
