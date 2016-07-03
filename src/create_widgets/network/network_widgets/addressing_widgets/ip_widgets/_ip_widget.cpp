#include "_ip_widget.h"

_IP_Widget::_IP_Widget(
        QWidget *parent,
        bool _IPv4HasDHCP,
        bool _IPv6HasDHCP) :
    _QWidget(parent)
{
    ipv6 = new QCheckBox("Use IPv6", this);
    staticRoute = new QCheckBox("Static Route", this);
    manageLayout = new QHBoxLayout();
    manageLayout->addWidget(ipv6);
    manageLayout->addWidget(staticRoute);
    manageWdg = new QWidget(this);
    manageWdg->setLayout(manageLayout);
    sets = new QStackedWidget(this);
    sets->addWidget(new _IPv4(this, _IPv4HasDHCP, 4));
    sets->addWidget(new _IPv6(this, _IPv6HasDHCP, 6));
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(manageWdg);
    commonLayout->addWidget(sets);
    setLayout(commonLayout);
    connect(ipv6, SIGNAL(toggled(bool)),
            this, SLOT(ipv6StateChanged(bool)));
    for (int i=0; i<sets->count(); i++) {
        _IPvX *wdg = static_cast<_IPvX*>(sets->widget(i));
        if ( nullptr==wdg ) continue;
        connect(staticRoute, SIGNAL(toggled(bool)),
                this, SLOT(staticRouteChanged(bool)));
        connect(wdg, SIGNAL(dhcpUsageChanged(uint, bool)),
                this, SLOT(dhcpUsageChanged(uint, bool)));
    };
}
void _IP_Widget::setTabIdx(int i)
{
    tabIdx = i;
}
void _IP_Widget::tabToClose()
{
    if ( -1<tabIdx ) {
        // uncheck DHCP if was checked
        _IPvX *wdg = static_cast<_IPvX*>(
                        sets->currentWidget());
        if ( nullptr!=wdg && wdg->getDHCPState() ) {
            emit dhcpUsageChanged(wdg->ver, tabIdx, false);
        };
    };
}

/* public slots */
QDomDocument _IP_Widget::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _addrElement;
    QString _tag;
    _tag = QString( ( staticRoute->isChecked() )? "route":"ip" );
    _addrElement = doc.createElement(_tag);
    if ( staticRoute->isChecked() ) {
        if ( ipv6->isChecked() )
            _addrElement.setAttribute("family", "ipv6");
        _IPvX *wdg = static_cast<_IPvX*>(sets->currentWidget());
        if ( nullptr!=wdg ) {
            _addrElement.setAttribute(
                        "address",
                        wdg->address->text());
            _addrElement.setAttribute(
                        "prefix",
                        wdg->prefix->value());
            _addrElement.setAttribute(
                        "gateway",
                        wdg->gateway->text());
            _addrElement.setAttribute(
                        "metric",
                        wdg->metric->value());
        };
    } else {
        if ( ipv6->isChecked() )
            _addrElement.setAttribute("family", "ipv6");
        _IPvX *wdg = static_cast<_IPvX*>(sets->currentWidget());
        if ( nullptr!=wdg ) {
            _addrElement.setAttribute(
                        "address",
                        wdg->address->text());
            _addrElement.setAttribute(
                        "prefix",
                        wdg->prefix->value());
            _addrElement.appendChild(
                        wdg->useDHCP->getDataDocument());
        };
    };
    doc.appendChild(_addrElement);
    return doc;
}
void _IP_Widget::setDataDescription(QString &_xmlDesc)
{

}
void _IP_Widget::updateDHCPUsage(uint ver, bool state)
{
    _IPvX *wdg = nullptr;
    switch (ver) {
    case 4:
        wdg = static_cast<_IPvX*>(sets->widget(0));
        break;
    case 6:
        wdg = static_cast<_IPvX*>(sets->widget(1));
        break;
    default:
        break;
    };
    if ( nullptr!=wdg )
        wdg->updateDHCPUsage(state);
}

/* private slots */
void _IP_Widget::ipv6StateChanged(bool state)
{
    sets->setCurrentIndex( (state)? 1:0 );
    // uncheck DHCP if was checked
    _IPvX *wdg = static_cast<_IPvX*>(
                    sets->widget( (state)? 0:1 ));
    if ( nullptr!=wdg && wdg->getDHCPState() ) {
        wdg->updateDHCPUsage(false);
        emit dhcpUsageChanged(wdg->ver, tabIdx, false);
    };
}
void _IP_Widget::dhcpUsageChanged(uint ver, bool state)
{
    if ( -1<tabIdx )
        emit dhcpUsageChanged(ver, tabIdx, state);
}
void _IP_Widget::staticRouteChanged(bool state)
{
    _IPvX *wdg = static_cast<_IPvX*>(
                    sets->currentWidget());
    if ( nullptr!=wdg ) {
        wdg->setStaticRouteMode(state);
    };
}
