#ifndef _IPVX_H
#define _IPVX_H

#include "dhcp_widget.h"
#include <QDebug>

class _IPvX : public _QWidget
{
    Q_OBJECT
public:
    explicit _IPvX(
            QWidget *parent = NULL,
            bool    *hasDHCP = NULL);
    QLineEdit       *address, *gateway;
    QSpinBox        *prefix, *metric;
    DHCP_Widget     *useDHCP;
    QVBoxLayout     *commonLayout;

signals:
    void             dhcpUsageChanged();

private:
    bool            *HasDHCP;
    QLabel          *addressL, *prefixL,
                    *gatewayL, *metricL;
    QGridLayout     *baselayout, *gatewayLayout;
    QWidget         *baseWidget, *gatewayWidget;

public slots:
    virtual void     setStaticRouteMode(bool);
    void             setGatewayEnabled(bool);
    void             setDHCPState(bool);
    void             setDHCPEnabled(bool);
    void             updateDHCPUsage();

private slots:
    void             dhcpStateChanged(bool);
};

#endif // _IPVX_H
