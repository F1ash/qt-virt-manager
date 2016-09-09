#ifndef UINT16_DATA_H
#define UINT16_DATA_H

#include "untyped_data.h"

class UINT16_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit UINT16_Data(
            QWidget *parent     = nullptr,
            QString  tag        = "",
            QString  topValue   = "65535");

signals:

public slots:
};

#endif // UINT16_DATA_H
