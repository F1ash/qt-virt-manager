#ifndef CEPH_SEC_TYPE_H
#define CEPH_SEC_TYPE_H

#include "_sec_type.h"
#include <QLineEdit>

class CephSecType : public _SecType
{
    Q_OBJECT
public:
    explicit CephSecType(
            QWidget        *parent  = NULL,
            virConnectPtr*  connPtr = NULL);
    QLineEdit      *usage;
    QHBoxLayout    *usageLayout;
    QWidget        *usageWdg;

public slots:
    QDomDocument    getSecStuff() const;
};

#endif // CEPH_SEC_TYPE_H
