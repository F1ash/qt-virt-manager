#ifndef REDIRDEV_DEVICE_EDIT_H
#define REDIRDEV_DEVICE_EDIT_H

#include "create_widgets/domain/common_widgets/redirdev_device.h"

class RedirDevDevice_Edit : public RedirDevDevice
{
    Q_OBJECT
public:
    explicit RedirDevDevice_Edit(
            QWidget        *parent     = nullptr,
            virConnectPtr*  connPtrPtr = nullptr);

signals:

public slots:
    void setDataDescription(const QString&);
};

#endif // REDIRDEV_DEVICE_EDIT_H
