#ifndef IP_SET_FLAGS_DATA_H
#define IP_SET_FLAGS_DATA_H

#include "untyped_data.h"
#include <QLineEdit>

class IPSETFLAGS_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit IPSETFLAGS_Data(
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

#endif // IP_SET_FLAGS_DATA_H
