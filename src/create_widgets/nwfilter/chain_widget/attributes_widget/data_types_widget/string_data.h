#ifndef STRING_DATA_H
#define STRING_DATA_H

#include "untyped_data.h"
#include <QLineEdit>

class STRING_Data : public UntypedData
{
    Q_OBJECT
public:
    explicit STRING_Data(
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

#endif // STRING_DATA_H
