#ifndef TCP_TUNNEL_H
#define TCP_TUNNEL_H

#include "create_widgets/domain/_qwidget.h"

class TCP_Tunnel : public _QWidget
{
    Q_OBJECT
public:
    explicit TCP_Tunnel(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QLabel          *addrLabel, *portLabel,
                    *macLabel, *typeLabel;
    QLineEdit       *addr, *mac;
    QSpinBox        *port;
    QComboBox       *type;
    QGridLayout     *commonLayout;

public slots:
    QDomDocument getDevDocument() const;
};

#endif // TCP_TUNNEL_H
