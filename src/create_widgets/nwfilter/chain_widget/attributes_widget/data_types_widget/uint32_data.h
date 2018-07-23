#ifndef UINT32_DATA_H
#define UINT32_DATA_H

#include "untyped_data.h"
#include <QDoubleSpinBox>

class UINT32_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit UINT32_Data(
            QWidget *parent     = Q_NULLPTR,
            QString  tag        = "",
            QString  topValue   = "4294967295");
    QString          getAttrValue() const;
    void             setAttrValue(const QString&);

private:
    QDoubleSpinBox  *data;

public slots:
    void             clearData();
};

#endif // UINT32_DATA_H
