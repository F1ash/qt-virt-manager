#ifndef UINT8_DATA_H
#define UINT8_DATA_H

#include "untyped_data.h"
#include <QSpinBox>

class UINT8_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit UINT8_Data(
            QWidget *parent     = nullptr,
            QString  tag        = "",
            QString  topValue   = "255");
    QString          getAttrValue() const;
    void             setAttrValue(const QString&);

private:
    QSpinBox        *data;

public slots:
    void             clearData();
};

#endif // UINT8_DATA_H
