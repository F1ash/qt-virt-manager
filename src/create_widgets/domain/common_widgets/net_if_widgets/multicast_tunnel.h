#ifndef MULTICAST_TUNNEL_H
#define MULTICAST_TUNNEL_H

#include "create_widgets/domain/_qwidget.h"
#include "mac_address.h"

class MultiCast_Tunnel : public _QWidget
{
    Q_OBJECT
public:
    explicit MultiCast_Tunnel(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QLabel          *addrLabel, *portLabel,
                    *typeLabel;
    QLineEdit       *addr;
    MAC_Address     *mac;
    QSpinBox        *port;
    QGridLayout     *commonLayout;

public slots:
    QDomDocument getDataDocument() const;
};

#endif // MULTICAST_TUNNEL_H
