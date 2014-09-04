#ifndef MULTICAST_TUNNEL_H
#define MULTICAST_TUNNEL_H

#include "create_widgets/domain/_qwidget.h"

class MultiCast_Tunnel : public _QWidget
{
    Q_OBJECT
public:
    explicit MultiCast_Tunnel(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QLabel          *addrLabel, *portLabel,
                    *macLabel, *typeLabel;
    QLineEdit       *addr, *mac;
    QSpinBox        *port;
    QGridLayout     *commonLayout;

public slots:
    QDomDocument getDevDocument() const;
};

#endif // MULTICAST_TUNNEL_H
