#ifndef _IPV6_H
#define _IPV6_H

#include "_ipvx.h"

class _IPv6 : public _IPvX
{
    Q_OBJECT
public:
    explicit _IPv6(
            QWidget *parent = NULL,
            bool    *hasDHCP = NULL);

private:

public slots:
    QDomDocument     getDataDocument() const;
    void             setStaticRouteMode(bool);
};

#endif // _IPV6_H
