#ifndef IPV6_ADDR_DATA_H
#define IPV6_ADDR_DATA_H

#include "untyped_data.h"

class IPv6_ADDR_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit IPv6_ADDR_Data(
            QWidget *parent     = nullptr,
            QString  tag        = "",
            QString  topValue   = "");

signals:

public slots:
};

#endif // IPV6_ADDR_DATA_H
