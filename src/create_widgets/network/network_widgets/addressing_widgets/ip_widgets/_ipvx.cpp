#include "_ipvx.h"

_IPvX::_IPvX(QWidget *parent, bool hasDHCP, uint _ver) :
    _QWidget(parent), ver(_ver)
{
    addressL = new QLabel("IP:", this);
    address = new QLineEdit(this);
    prefixL = new QLabel("Prefix:", this);
    prefix = new QSpinBox(this);
    prefix->setRange(0, 65535);
    baselayout = new QGridLayout();
    baselayout->addWidget(addressL, 0, 0);
    baselayout->addWidget(address, 0, 1);
    baselayout->addWidget(prefixL, 1, 0);
    baselayout->addWidget(prefix, 1, 1);
    baseWidget = new QWidget(this);
    baseWidget->setLayout(baselayout);
    gatewayL = new QLabel("Gateway:", this);
    gateway = new QLineEdit(this);
    metricL = new QLabel("Metric:", this);
    metric = new QSpinBox(this);
    metric->setRange(0, 100);
    gatewayLayout = new QGridLayout();
    gatewayLayout->addWidget(gatewayL, 0, 0);
    gatewayLayout->addWidget(gateway, 0, 1);
    gatewayLayout->addWidget(metricL, 1, 0);
    gatewayLayout->addWidget(metric, 1, 1);
    gatewayWidget = new QWidget(this);
    gatewayWidget->setLayout(gatewayLayout);
    gatewayWidget->setEnabled(false);
    useDHCP = new DHCP_Widget(this, "Use DHCP");
    useDHCP->setToolTip(
    "WARNING: allows one (each IPv4 and IPv6)\n\
definition a DHCP server on the network");
    updateDHCPUsage(hasDHCP);
    //qDebug()<<(*HasDHCP);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWidget);
    commonLayout->addWidget(gatewayWidget);
    commonLayout->addWidget(useDHCP);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(useDHCP, SIGNAL(toggled(bool)),
            this, SLOT(dhcpStateChanged(bool)));
}
bool _IPvX::getDHCPState() const
{
    return useDHCP->isUsed();
}

/* public slots */
void _IPvX::setStaticRouteMode(bool state)
{
}
void _IPvX::setGatewayEnabled(bool state)
{
    gatewayWidget->setEnabled(state);
    updateDHCPUsage(state);
}
void _IPvX::updateDHCPUsage(bool state)
{
    useDHCP->setFreez(state);
    useDHCP->setCheckState( Qt::Unchecked );
}

/* private slots */
void _IPvX::dhcpStateChanged(bool state)
{
    emit dhcpUsageChanged(ver, state);
}
