#ifndef _IPV4_H
#define _IPV4_H

#include "_ipvx.h"

class _IPv4 : public _IPvX
{
    Q_OBJECT
public:
    explicit _IPv4(
            QWidget *parent  = nullptr,
            bool     hasDHCP = false,
            uint     _ver    = 0);

private:

public slots:
    void             setStaticRouteMode(bool);
};

#endif // _IPV4_H
