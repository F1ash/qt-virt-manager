#ifndef BRIDGE_TO_LAN_H
#define BRIDGE_TO_LAN_H

#include "create_widgets/domain/_qwidget.h"
#include "mac_address.h"
#include "virtual_port.h"

class Bridge_to_LAN : public _QWidget
{
    Q_OBJECT
public:
    explicit Bridge_to_LAN(
            QWidget *parent = NULL,
            virConnectPtr conn = NULL);

private:
    QLabel          *bridgeLabel, *targetLabel;
    QLineEdit       *bridge, *target;
    QGridLayout     *baseLayout;
    QWidget         *baseWdg;
    MAC_Address     *mac;
    VirtualPort     *virtPort;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);
};

#endif // BRIDGE_TO_LAN_H
