#ifndef MAC_ADDR_DATA_H
#define MAC_ADDR_DATA_H

#include "untyped_data.h"

class MAC_ADDR_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit MAC_ADDR_Data(
            QWidget *parent     = nullptr,
            QString  tag        = "",
            QString  topValue   = "");

signals:

public slots:
};

#endif // MAC_ADDR_DATA_H
