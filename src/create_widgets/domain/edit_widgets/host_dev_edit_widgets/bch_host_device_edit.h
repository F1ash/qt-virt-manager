#ifndef BCH_HOST_DEVICE_EDIT_H
#define BCH_HOST_DEVICE_EDIT_H

#include "create_widgets/domain/common_widgets/host_dev_widgets/bch_host_device.h"

class BCh_Host_Device_Edit : public BCh_Host_Device
{
    Q_OBJECT
public:
    explicit BCh_Host_Device_Edit(QWidget *parent = nullptr);

signals:

public slots:
    void     setDataDescription(const QString&);
};

#endif // BCH_HOST_DEVICE_EDIT_H
