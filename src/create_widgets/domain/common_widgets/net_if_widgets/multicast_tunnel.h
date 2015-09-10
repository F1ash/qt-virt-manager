#ifndef MULTICAST_TUNNEL_H
#define MULTICAST_TUNNEL_H

#include "create_widgets/domain/_qwidget.h"
#include "mac_address.h"
#include "create_widgets/domain/common_widgets/device_address.h"

class MultiCast_Tunnel : public _QWidget
{
    Q_OBJECT
public:
    explicit MultiCast_Tunnel(
            QWidget        *parent  = NULL,
            virConnectPtr*  connPtr = NULL);

private:
    QLabel          *addrLabel, *portLabel,
                    *typeLabel;
    QLineEdit       *ipAddr;
    QSpinBox        *port;
    QGridLayout     *baseLayout;
    QWidget         *baseWdg;
    MAC_Address     *mac;
    DeviceAddress   *addr;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);
};

#endif // MULTICAST_TUNNEL_H
