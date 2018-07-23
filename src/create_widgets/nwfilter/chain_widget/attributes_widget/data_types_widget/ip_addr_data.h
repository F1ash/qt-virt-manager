#ifndef IP_ADDR_DATA_H
#define IP_ADDR_DATA_H

#include "untyped_data.h"
#include <QLineEdit>

class IP_ADDR_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit IP_ADDR_Data(
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

#endif // IP_ADDR_DATA_H
