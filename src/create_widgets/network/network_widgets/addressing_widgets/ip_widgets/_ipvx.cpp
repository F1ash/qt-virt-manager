#include "_ipvx.h"

_IPvX::_IPvX(QWidget *parent, bool *hasDHCP) :
    _QWidget(parent), HasDHCP(hasDHCP)
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
    useDHCP->setDisabled(*HasDHCP);
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

/* public slots */
void _IPvX::setStaticRouteMode(bool state)
{
}
void _IPvX::setGatewayEnabled(bool state)
{
    gatewayWidget->setEnabled(state);
}
void _IPvX::setDHCPState(bool state)
{
    useDHCP->setUsage(state);
}
void _IPvX::setDHCPEnabled(bool state)
{
    useDHCP->setCheckState( Qt::Unchecked );
    if ( !(*HasDHCP) ) useDHCP->setFreez(!state);
}
void _IPvX::updateDHCPUsage()
{
    if ( *HasDHCP ) {
        useDHCP->setFreez(!useDHCP->isUsed());
    } else {
        useDHCP->setFreez(false);
    };
}

/* private slots */
void _IPvX::dhcpStateChanged(bool state)
{
    *HasDHCP = state;
    //qDebug()<<(*HasDHCP)<<objectName();
    emit dhcpUsageChanged();
}
