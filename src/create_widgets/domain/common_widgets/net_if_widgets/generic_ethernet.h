#ifndef GENERIC_ETHERNET_H
#define GENERIC_ETHERNET_H

#include "create_widgets/domain/_qwidget.h"
#include "create_widgets/domain/common_widgets/device_address.h"

class Generic_Ethernet : public _QWidget
{
    Q_OBJECT
public:
    explicit Generic_Ethernet(
            QWidget *parent = NULL,
            virConnectPtr conn = NULL);

private:
    QLabel          *targetLabel, *scriptLabel;
    QLineEdit       *target, *script;
    QGridLayout     *baseLayout;
    QWidget         *baseWdg;
    DeviceAddress   *addr;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);
};

#endif // GENERIC_ETHERNET_H
