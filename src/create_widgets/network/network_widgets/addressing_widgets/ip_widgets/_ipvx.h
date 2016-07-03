#ifndef _IPVX_H
#define _IPVX_H

#include "dhcp_widget.h"
#include <QDebug>

class _IPvX : public _QWidget
{
    Q_OBJECT
public:
    explicit _IPvX(
            QWidget *parent  = nullptr,
            bool     hasDHCP = false,
            uint     _ver    = 0);
    QLineEdit       *address, *gateway;
    QSpinBox        *prefix, *metric;
    DHCP_Widget     *useDHCP;
    QVBoxLayout     *commonLayout;
    bool             getDHCPState() const;
    const uint       ver;

signals:
    void             dhcpUsageChanged(uint, bool);

private:
    QLabel          *addressL, *prefixL,
                    *gatewayL, *metricL;
    QGridLayout     *baselayout, *gatewayLayout;
    QWidget         *baseWidget, *gatewayWidget;

public slots:
    virtual void     setStaticRouteMode(bool);
    void             setGatewayEnabled(bool);
    void             updateDHCPUsage(bool);

private slots:
    virtual void     dhcpStateChanged(bool);
};

#endif // _IPVX_H
