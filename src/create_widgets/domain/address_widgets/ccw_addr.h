#ifndef CCW_ADDR_H
#define CCW_ADDR_H

#include "_addr.h"
#include <QSpinBox>

class CCWAddr : public _Addr
{
    Q_OBJECT
public:
    explicit CCWAddr(QWidget *parent = Q_NULLPTR);

private:
    QLabel          *machineLabel;
    QLabel          *cssidLabel;
    QLabel          *ssidLabel;
    QLabel          *devnoLabel;
    QLineEdit       *machine;
    QLineEdit       *cssid;
    QSpinBox        *ssid;
    QLineEdit       *devno;
    QGridLayout     *commonlayout;

public slots:
    AttrList getAttrList() const;
};

#endif // CCW_ADDR_H
