#ifndef IP_MASK_DATA_H
#define IP_MASK_DATA_H

#include "untyped_data.h"
#include <QLineEdit>

class IP_MASK_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit IP_MASK_Data(
            QWidget *parent     = Q_NULLPTR,
            QString  tag        = "",
            QString  topValue   = "");
    QString          getAttrValue() const;
    void             setAttrValue(const QString&);

private:
    QLineEdit       *data;

public slots:
    void             clearData();
};

#endif // IP_MASK_DATA_H
