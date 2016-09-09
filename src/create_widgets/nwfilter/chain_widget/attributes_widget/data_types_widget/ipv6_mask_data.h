#ifndef IPV6_MASK_DATA_H
#define IPV6_MASK_DATA_H

#include "untyped_data.h"

class IPv6_MASK_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit IPv6_MASK_Data(
            QWidget *parent     = nullptr,
            QString  tag        = "",
            QString  topValue   = "");

signals:

public slots:
};

#endif // IPV6_MASK_DATA_H
