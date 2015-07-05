#ifndef CEPH_SEC_TYPE_H
#define CEPH_SEC_TYPE_H

#include "_sec_type.h"

class CephSecType : public _SecType
{
    Q_OBJECT
public:
    explicit CephSecType(
            QWidget         *parent = NULL,
            virConnectPtr    _conn  = NULL);

signals:

public slots:
};

#endif // CEPH_SEC_TYPE_H
