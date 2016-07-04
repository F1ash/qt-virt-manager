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
        if ( nullptr!=wdg && wdg->getDHCPUsageState() ) {
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
                        wdg->significantBitsL->currentText(),
                        wdg->significantBits->text());
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
                        wdg->significantBitsL->currentText(),
                        wdg->significantBits->text());
            _addrElement.appendChild(
                        wdg->useDHCP->getDataDocument());
        };
    };
    doc.appendChild(_addrElement);
    return doc;
}
void _IP_Widget::setDataDescription(QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _el = doc.documentElement();
    QString _ver = _el.attribute("family");
    ipv6->setChecked( _ver=="ipv6" );
    _IPvX *wdg = static_cast<_IPvX*>(
                    sets->currentWidget());
    if ( nullptr!=wdg ) {
        wdg->address->setText(
                    _el.attribute("address"));
        if ( _el.hasAttribute("netmask") ) {
            wdg->significantBitsL->setCurrentIndex(0);
            wdg->significantBits->setText(
                        _el.attribute("netmask"));
        } else if ( _el.hasAttribute("prefix") ) {
            wdg->significantBitsL->setCurrentIndex(1);
            wdg->significantBits->setText(
                        _el.attribute("prefix"));
        };
        if        ( _el.tagName()=="ip" ) {
            // if dhcp used then set it
            QDomElement _dhcp = _el.firstChildElement("dhcp");
            if ( !_dhcp.isNull() ) {
                QDomDocument _doc;
                _doc.setContent(QString());
                _doc.appendChild(_dhcp.cloneNode());
                QString _xml = _doc.toString();
                wdg->useDHCP->setDataDescription(_xml);
            };
        } else if ( _el.tagName()=="route" ) {
            // set gateway and metric
            staticRoute->setChecked(true);
            wdg->gateway->setText(
                        _el.attribute("gateway"));
            wdg->metric->setValue(
                        _el.attribute("metric").toInt());
        } else {
            // error
            wdg->address->setText("Data error");
        };
    };
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
    // uncheck StateRoute
    //staticRoute->setChecked(false);
    sets->setCurrentIndex( (state)? 1:0 );
    // uncheck DHCP if was checked
    _IPvX *wdg = static_cast<_IPvX*>(
                    sets->widget( (state)? 0:1 ));
    if ( nullptr!=wdg && wdg->getDHCPUsageState() ) {
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
