#ifndef _IPVX_H
#define _IPVX_H

#include "dhcp_widget.h"
#include <QComboBox>
#include <QDebug>

class _IPvX : public _QWidget
{
    Q_OBJECT
public:
    explicit _IPvX(
            QWidget *parent  = nullptr,
            bool     hasDHCP = false,
            uint     _ver    = 0);
    QComboBox       *significantBitsL;
    QLineEdit       *address, *gateway, *significantBits;
    QSpinBox        *metric;
    DHCP_Widget     *useDHCP;
    QVBoxLayout     *commonLayout;
    bool             getDHCPUsageState() const;
    bool             isNetworkHasDHCP() const;
    const uint       ver;

signals:
    void             dhcpUsageChanged(uint, bool);

private:
    bool             networkHasDHCP;
    QLabel          *addressL,
                    *gatewayL, *metricL;
    QGridLayout     *baselayout, *gatewayLayout;
    QWidget         *baseWidget, *gatewayWidget;

public slots:
    virtual void     setStaticRouteMode(bool);
    void             setGatewayEnabled(bool);
    void             updateDHCPUsage(bool);

private slots:
    virtual void     dhcpStateChanged(bool);
    void             significantBitsChanged(int);
};

#endif // _IPVX_H
