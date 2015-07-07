#ifndef ISCSI_SEC_TYPE_H
#define ISCSI_SEC_TYPE_H

#include "_sec_type.h"
#include <QLineEdit>

class iSCSISecType : public _SecType
{
    Q_OBJECT
public:
    explicit iSCSISecType(
            QWidget         *parent = NULL,
            virConnectPtr    _conn = NULL);
    QLineEdit      *usage;
    QHBoxLayout    *usageLayout;
    QWidget        *usageWdg;

public slots:
    QDomDocument    getSecStuff() const;
};

#endif // ISCSI_SEC_TYPE_H
