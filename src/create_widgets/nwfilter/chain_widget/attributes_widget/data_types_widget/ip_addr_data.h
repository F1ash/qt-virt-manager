#ifndef IP_ADDR_DATA_H
#define IP_ADDR_DATA_H

#include "untyped_data.h"

class IP_ADDR_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit IP_ADDR_Data(
            QWidget *parent     = nullptr,
            QString  tag        = "",
            QString  topValue   = "");

signals:

public slots:
};

#endif // IP_ADDR_DATA_H
