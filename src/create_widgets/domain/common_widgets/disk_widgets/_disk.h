#ifndef _DISK_H
#define _DISK_H

#include "create_widgets/domain/_qwidget.h"
#include "_hosts.h"
#include "_target.h"
#include "_device.h"
#include "_read_only.h"
#include "_startup_policy.h"
#include "_use_encryption.h"
#include "create_widgets/domain/common_widgets/sec_label_widgets/sec_labels.h"
#include "create_widgets/domain/common_widgets/device_address.h"

class _Disk : public _QWidget
{
    Q_OBJECT
public:
    explicit _Disk(
            QWidget         *parent = NULL,
            virConnectPtr    conn = NULL);
    _Hosts              *hosts;
    _StartupPolicy      *startupPolicy;
    _Device             *devType;
    _Target             *target;
    SecLabels           *secLabels;
    _ReadOnly           *readOnly;
    _UseEncryption      *encrypt;
    DeviceAddress       *addr;
    QGridLayout         *baseLayout;
    QWidget             *baseWdg;
    QVBoxLayout         *commonLayout;
};

#endif // _DISK_H
