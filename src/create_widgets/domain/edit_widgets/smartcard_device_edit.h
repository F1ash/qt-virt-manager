#ifndef SMARTCARD_DEVICE_EDIT_H
#define SMARTCARD_DEVICE_EDIT_H

#include "create_widgets/domain/common_widgets/smartcard_device.h"

class SmartCardDevice_Edit : public SmartCardDevice
{
    Q_OBJECT
public:
    explicit SmartCardDevice_Edit(QWidget *parent = 0);

signals:

public slots:
    void setDeviceData(QString&);
};

#endif // SMARTCARD_DEVICE_EDIT_H
