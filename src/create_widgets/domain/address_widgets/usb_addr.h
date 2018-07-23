#ifndef USB_ADDR_H
#define USB_ADDR_H

#include "_addr.h"

class USBAddr : public _Addr
{
    Q_OBJECT
public:
    explicit USBAddr(QWidget *parent = Q_NULLPTR);
    QLineEdit       *bus;
    QLineEdit       *port;

private:
    QLabel          *busLabel;
    QLabel          *portLabel;
    QGridLayout     *commonlayout;

public slots:
    AttrList getAttrList() const;
};

#endif // USB_ADDR_H
