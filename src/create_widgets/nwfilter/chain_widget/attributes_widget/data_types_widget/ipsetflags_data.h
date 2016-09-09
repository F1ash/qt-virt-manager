#ifndef IP_SET_FLAGS_DATA_H
#define IP_SET_FLAGS_DATA_H

#include "untyped_data.h"

class IPSETFLAGS_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit IPSETFLAGS_Data(
            QWidget *parent     = nullptr,
            QString  tag        = "",
            QString  topValue   = "");

signals:

public slots:
};

#endif // IP_SET_FLAGS_DATA_H
