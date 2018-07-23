#ifndef MAC_MASK_DATA_H
#define MAC_MASK_DATA_H

#include "untyped_data.h"
#include <QLineEdit>

class MAC_MASK_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit MAC_MASK_Data(
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

#endif // MAC_MASK_DATA_H
