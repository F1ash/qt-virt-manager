#ifndef IP_MASK_DATA_H
#define IP_MASK_DATA_H

#include "untyped_data.h"

class IP_MASK_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit IP_MASK_Data(
            QWidget *parent     = nullptr,
            QString  tag        = "",
            QString  topValue   = "");

signals:

public slots:
};

#endif // IP_MASK_DATA_H
