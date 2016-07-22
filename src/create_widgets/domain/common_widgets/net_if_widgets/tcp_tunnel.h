#ifndef TCP_TUNNEL_H
#define TCP_TUNNEL_H

#include "create_widgets/common/_qwidget.h"
#include "mac_address.h"
#include "create_widgets/domain/common_widgets/device_address.h"

class TCP_Tunnel : public _QWidget
{
    Q_OBJECT
public:
    explicit TCP_Tunnel(
            QWidget        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr);

private:
    QLabel          *addrLabel, *portLabel,
                    *typeLabel;
    QLineEdit       *ipAddr;
    QSpinBox        *port;
    QComboBox       *type;
    QGridLayout     *baseLayout;
    QWidget         *baseWdg;
    MAC_Address     *mac;
    DeviceAddress   *addr;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
};

#endif // TCP_TUNNEL_H
