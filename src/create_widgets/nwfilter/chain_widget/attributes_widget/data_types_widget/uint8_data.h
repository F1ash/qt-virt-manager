#ifndef UINT8_DATA_H
#define UINT8_DATA_H

#include "untyped_data.h"

class UINT8_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit UINT8_Data(
            QWidget *parent     = nullptr,
            QString  tag        = "",
            QString  topValue   = "255");

signals:

public slots:
};

#endif // UINT8_DATA_H
