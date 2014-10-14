#ifndef TCP_TUNNEL_H
#define TCP_TUNNEL_H

#include "create_widgets/domain/_qwidget.h"
#include "mac_address.h"

class TCP_Tunnel : public _QWidget
{
    Q_OBJECT
public:
    explicit TCP_Tunnel(
            QWidget *parent = NULL,
            virConnectPtr conn = NULL);

private:
    QLabel          *addrLabel, *portLabel,
                    *typeLabel;
    QLineEdit       *addr;
    QSpinBox        *port;
    QComboBox       *type;
    MAC_Address     *mac;
    QGridLayout     *commonLayout;

public slots:
    QDomDocument getDataDocument() const;
};

#endif // TCP_TUNNEL_H
