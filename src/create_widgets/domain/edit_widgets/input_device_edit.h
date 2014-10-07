#ifndef INPUT_DEVICE_EDIT_H
#define INPUT_DEVICE_EDIT_H

#include "create_widgets/domain/common_widgets/input_device.h"

class InputDevice_Edit : public InputDevice
{
    Q_OBJECT
public:
    explicit InputDevice_Edit(QWidget *parent = 0);

signals:

public slots:
    void setDeviceData(QString&);
};

#endif // INPUT_DEVICE_EDIT_H
