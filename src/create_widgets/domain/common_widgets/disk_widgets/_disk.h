#ifndef _DISK_H
#define _DISK_H

#include "create_widgets/domain/_qwidget.h"
#include "_hosts.h"
#include "_target.h"
#include "_device.h"
#include "_read_only.h"
#include "_startup_policy.h"

class _Disk : public _QWidget
{
    Q_OBJECT
public:
    explicit _Disk(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);
    _Hosts          *hosts;
    _StartupPolicy  *startupPolicy;
    _Device         *devType;
    _Target         *target;
    _ReadOnly       *readOnly;
    QGridLayout     *baseLayout;
    QWidget         *baseWdg;
    QVBoxLayout     *commonLayout;

public slots:
    virtual QDomDocument getDevDocument() const;
};

#endif // _DISK_H
