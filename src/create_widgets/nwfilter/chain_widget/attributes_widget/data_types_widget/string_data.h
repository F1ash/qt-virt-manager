#ifndef STRING_DATA_H
#define STRING_DATA_H

#include "untyped_data.h"

class STRING_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit STRING_Data(
            QWidget *parent     = nullptr,
            QString  tag        = "",
            QString  topValue   = "");

signals:

public slots:
};

#endif // STRING_DATA_H
