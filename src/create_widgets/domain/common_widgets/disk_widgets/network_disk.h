#ifndef NETWORK_DISK_H
#define NETWORK_DISK_H

#include "create_widgets/domain/_qwidget.h"
#include "_target.h"
#include "_device.h"
#include "_read_only.h"
#include "_startup_policy.h"
#include "_hosts.h"

class Network_Disk : public _QWidget
{
    Q_OBJECT
public:
    explicit Network_Disk(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QLabel          *protocolLabel, *sourceNameLabel;
    QComboBox       *protocol;
    QLineEdit       *sourceName;
    _Hosts          *hosts;
    _StartupPolicy  *startupPolicy;
    _Device         *devType;
    _Target         *target;
    _ReadOnly       *readOnly;
    QGridLayout     *baseLayout;
    QWidget         *baseWdg;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument getDevDocument() const;

private slots:
    void protocolTypeChanged(int);
};

#endif // NETWORK_DISK_H
