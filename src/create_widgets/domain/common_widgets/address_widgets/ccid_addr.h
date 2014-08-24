#ifndef CCID_ADDR_H
#define CCID_ADDR_H

#include "_addr.h"

class CCIDAddr : public _Addr
{
    Q_OBJECT
public:
    explicit CCIDAddr(QWidget *parent = 0);

private:
    QLabel          *busLabel;
    QLabel          *slotLabel;
    QLineEdit       *bus;
    QLineEdit       *slot;
    QGridLayout     *commonlayout;

public slots:
    AttrList getAttrList() const;
};

#endif // CCID_ADDR_H
