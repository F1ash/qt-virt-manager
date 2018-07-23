#ifndef GENERIC_ETHERNET_H
#define GENERIC_ETHERNET_H

#include "create_widgets/common/_qwidget.h"
#include "create_widgets/domain/device_address.h"

class Generic_Ethernet : public _QWidget
{
    Q_OBJECT
public:
    explicit Generic_Ethernet(
            QWidget        *parent      = Q_NULLPTR,
            virConnectPtr*  connPtrPtr  = Q_NULLPTR);

private:
    QLabel          *targetLabel, *scriptLabel;
    QLineEdit       *target, *script;
    QGridLayout     *baseLayout;
    QWidget         *baseWdg;
    DeviceAddress   *addr;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
};

#endif // GENERIC_ETHERNET_H
