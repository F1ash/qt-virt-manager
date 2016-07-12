#include "_ipvx.h"

_IPvX::_IPvX(QWidget *parent, bool hasDHCP, uint _ver) :
    _QWidget(parent), ver(_ver)
{
    networkHasDHCP = hasDHCP;
    addressL = new QLabel("IP:", this);
    address = new QLineEdit(this);
    significantBitsL = new QComboBox(this);
    significantBitsL->addItems(
                QStringList()<<"netmask"<<"prefix");
    significantBits = new QLineEdit(this);
    baselayout = new QGridLayout();
    baselayout->addWidget(addressL, 0, 0);
    baselayout->addWidget(address, 0, 1);
    baselayout->addWidget(significantBitsL, 1, 0);
    baselayout->addWidget(significantBits, 1, 1);
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
    useDHCP->setUsageToolTip(
    "WARNING: allows one (each IPv4 and IPv6)\n\
definition a DHCP server on the network");
    updateDHCPUsage(networkHasDHCP);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWidget);
    commonLayout->addWidget(gatewayWidget);
    commonLayout->addWidget(useDHCP);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(useDHCP, SIGNAL(toggled(bool)),
            this, SLOT(dhcpStateChanged(bool)));
    connect(significantBitsL, SIGNAL(currentIndexChanged(int)),
            this, SLOT(significantBitsChanged(int)));
    connect(address, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(significantBits, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
}
bool _IPvX::getDHCPUsageState() const
{
    return useDHCP->isUsed();
}
bool _IPvX::isNetworkHasDHCP() const
{
    return networkHasDHCP;
}
void _IPvX::withoutGateway(bool state)
{
    gatewayWidget->setVisible(!state);
}
bool _IPvX::IP_data_isEmpty() const
{
    return ( address->text().isEmpty() ||
             significantBits->text().isEmpty() );
}

/* public slots */
void _IPvX::setStaticRouteMode(bool state)
{
}
void _IPvX::setGatewayEnabled(bool state)
{
    gatewayWidget->setEnabled(state);
    if ( !state && networkHasDHCP ) {
        return;
    };
    useDHCP->setFreez(state);
}
void _IPvX::updateDHCPUsage(bool state)
{
    networkHasDHCP = state;
    useDHCP->setFreez(state);
}

/* private slots */
void _IPvX::dhcpStateChanged(bool state)
{
    emit dhcpUsageChanged(ver, state);
}
void _IPvX::significantBitsChanged(int idx)
{
    QString p;
    switch (ver) {
    case 4:
        if ( idx==0 ) {
            p.append("255.255.255.0");
        } else {
            p.append("24");
        };
        break;
    case 6:
        if ( idx==0 ) {
            p.append("FFFF:FFFF:FFFF:FFFF:0000:0000:0000:0000");
        } else {
            p.append("64");
        };
        break;
    default:
        p.append("error");
        break;
    };
    significantBits->clear();
    significantBits->setPlaceholderText(p);
}
