#ifndef TLS_SEC_TYPE_H
#define TLS_SEC_TYPE_H

#include "_sec_type.h"
#include <QLineEdit>

class tlsSecType : public _SecType
{
    Q_OBJECT
public:
    explicit tlsSecType(
            QWidget        *parent     = Q_NULLPTR,
            virConnectPtr*  connPtrPtr = Q_NULLPTR);
    QLineEdit      *usage;
    QHBoxLayout    *usageLayout;
    QWidget        *usageWdg;

public slots:
    QDomDocument    getSecStuff() const;
};

#endif // TLS_SEC_TYPE_H
