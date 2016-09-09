#ifndef MAC_MASK_DATA_H
#define MAC_MASK_DATA_H

#include "untyped_data.h"

class MAC_MASK_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit MAC_MASK_Data(
            QWidget *parent     = nullptr,
            QString  tag        = "",
            QString  topValue   = "");

signals:

public slots:
};

#endif // MAC_MASK_DATA_H
