#ifndef HUB_DEVICE_EDIT_H
#define HUB_DEVICE_EDIT_H

#include "create_widgets/domain/common_widgets/hub_device.h"

class HubDevice_Edit : public HubDevice
{
    Q_OBJECT
public:
    explicit HubDevice_Edit(QWidget *parent = Q_NULLPTR);

signals:

public slots:
    void setDataDescription(const QString&);

};

#endif // HUB_DEVICE_EDIT_H
