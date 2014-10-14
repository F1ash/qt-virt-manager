#ifndef VIRTIOSERIAL_ADDR_H
#define VIRTIOSERIAL_ADDR_H

#include "_addr.h"

class VirtioSerialAddr : public _Addr
{
    Q_OBJECT
public:
    explicit VirtioSerialAddr(QWidget *parent = NULL);

private:
    QLabel          *contrLabel;
    QLabel          *busLabel;
    QLabel          *slotLabel;
    QLineEdit       *controller;
    QLineEdit       *bus;
    QLineEdit       *slot;
    QGridLayout     *commonlayout;

public slots:
    AttrList getAttrList() const;

};

#endif // VIRTIOSERIAL_ADDR_H
