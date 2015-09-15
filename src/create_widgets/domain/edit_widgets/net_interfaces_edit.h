#ifndef NET_INTERFACES_EDIT_H
#define NET_INTERFACES_EDIT_H

#include "create_widgets/domain/common_widgets/net_interfaces.h"

class NetInterfaces_Edit : public NetInterfaces
{
    Q_OBJECT
public:
    explicit NetInterfaces_Edit(
            QWidget        *parent  = NULL,
            virConnectPtr*  connPtrPtr = NULL);

signals:

public slots:
    void         setDataDescription(QString&);
};

#endif // NET_INTERFACES_EDIT_H
