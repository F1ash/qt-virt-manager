#ifndef UINT16_DATA_H
#define UINT16_DATA_H

#include "untyped_data.h"
#include <QSpinBox>

class UINT16_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit UINT16_Data(
            QWidget *parent     = nullptr,
            QString  tag        = "",
            QString  topValue   = "65535");
    QString          getAttrValue() const;
    void             setAttrValue(const QString&);

private:
    QSpinBox        *data;

public slots:
    void             clearData();
};

#endif // UINT16_DATA_H
