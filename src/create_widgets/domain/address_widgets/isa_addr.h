#ifndef ISA_ADDR_H
#define ISA_ADDR_H

#include "_addr.h"

class IsaAddr : public _Addr
{
    Q_OBJECT
public:
    explicit IsaAddr(QWidget *parent = nullptr);

private:
    QLabel          *iobaseLabel;
    QLabel          *irqLabel;
    QLineEdit       *iobase;
    QLineEdit       *irq;
    QGridLayout     *commonlayout;

public slots:
    AttrList getAttrList() const;
};

#endif // ISA_ADDR_H
