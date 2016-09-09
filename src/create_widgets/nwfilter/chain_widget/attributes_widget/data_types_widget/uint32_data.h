#ifndef UINT32_DATA_H
#define UINT32_DATA_H

#include "untyped_data.h"

class UINT32_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit UINT32_Data(
            QWidget *parent     = nullptr,
            QString  tag        = "",
            QString  topValue   = "4294967295");

signals:

public slots:
};

#endif // UINT32_DATA_H
