#ifndef INPUT_DEVICE_EDIT_H
#define INPUT_DEVICE_EDIT_H

#include "create_widgets/domain/common_widgets/input_device.h"

class InputDevice_Edit : public InputDevice
{
    Q_OBJECT
public:
    explicit InputDevice_Edit(QWidget *parent = nullptr);

signals:

public slots:
    void setDataDescription(const QString&);
};

#endif // INPUT_DEVICE_EDIT_H
