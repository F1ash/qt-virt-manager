#ifndef BOOLEAN_DATA_H
#define BOOLEAN_DATA_H

#include "untyped_data.h"

class BOOLEAN_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit BOOLEAN_Data(
            QWidget *parent     = nullptr,
            QString  tag        = "",
            QString  topValue   = "yes");

signals:

public slots:
};

#endif // BOOLEAN_DATA_H
