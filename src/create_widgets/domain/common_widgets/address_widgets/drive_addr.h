#ifndef DRIVE_ADDR_H
#define DRIVE_ADDR_H

#include "_addr.h"

class DriveAddr : public _Addr
{
    Q_OBJECT
public:
    explicit DriveAddr(QWidget *parent = 0);

signals:

private:
    QLabel          *contrLabel;
    QLabel          *busLabel;
    QLabel          *targetLabel;
    QLabel          *unitLabel;
    QLineEdit       *controller;
    QLineEdit       *bus;
    QLineEdit       *target;
    QLineEdit       *unit;
    QGridLayout     *commonlayout;

public slots:
    AttrList getAttrList() const;
};

#endif // DRIVE_ADDR_H
