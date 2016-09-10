#ifndef MAC_ADDR_DATA_H
#define MAC_ADDR_DATA_H

#include "untyped_data.h"
#include <QLineEdit>

class MAC_ADDR_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit MAC_ADDR_Data(
            QWidget *parent     = nullptr,
            QString  tag        = "",
            QString  topValue   = "");
    QString          getAttrValue() const;
    void             setAttrValue(const QString&);

private:
    QLineEdit       *data;

public slots:
    void             clearData();
};

#endif // MAC_ADDR_DATA_H
