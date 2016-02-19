#ifndef PCI_ADDR_H
#define PCI_ADDR_H

#include "_addr.h"
#include <QSpinBox>
#include <QCheckBox>

class PciAddr : public _Addr
{
    Q_OBJECT
public:
    explicit PciAddr(QWidget *parent = nullptr);
    QLineEdit       *domain;
    QLineEdit       *bus;
    QLineEdit       *slot;
    QSpinBox        *function;
    QCheckBox       *multifunction;

private:
    QLabel          *domainLabel;
    QLabel          *busLabel;
    QLabel          *slotLabel;
    QLabel          *functLabel;
    QGridLayout     *commonlayout;

public slots:
    AttrList         getAttrList() const;

private slots:
    void             usageChanged(int);
};

#endif // PCI_ADDR_H
