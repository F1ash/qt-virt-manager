#ifndef ISCSI_SEC_TYPE_H
#define ISCSI_SEC_TYPE_H

#include "_sec_type.h"

class iSCSISecType : public _SecType
{
    Q_OBJECT
public:
    explicit iSCSISecType(
            QWidget         *parent = NULL,
            virConnectPtr    _conn = NULL);

signals:

public slots:
};

#endif // ISCSI_SEC_TYPE_H
