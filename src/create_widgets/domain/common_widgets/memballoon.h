#ifndef MEMBALLOON_H
#define MEMBALLOON_H

#include "create_widgets/domain/_qwidget.h"
#include "address_widgets/pci_addr.h"

class MemBalloon : public _QWidget
{
    Q_OBJECT
public:
    explicit MemBalloon(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);
    QComboBox       *model;
    QCheckBox       *periodLabel, *addrLabel;
    QSpinBox        *period;
    PciAddr         *addr;

private:
    QLabel          *modelLabel;
    QGridLayout     *baseLayout;
    QWidget         *baseWdg;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDevDocument() const;

private slots:
    void modelChanged(QString);
};

#endif // MEMBALLOON_H
