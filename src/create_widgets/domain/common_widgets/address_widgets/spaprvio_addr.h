#ifndef SPAPRVIO_ADDR_H
#define SPAPRVIO_ADDR_H

#include "_addr.h"

class SpaprVioAddr : public _Addr
{
    Q_OBJECT
public:
    explicit SpaprVioAddr(QWidget *parent = 0);

private:
    QLabel          *regLabel;
    QLineEdit       *reg;
    QGridLayout     *commonlayout;

public slots:
    AttrList getAttrList() const;
};

#endif // SPAPRVIO_ADDR_H
