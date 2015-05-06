#include "_ip_widget.h"

_IP_Widget::_IP_Widget(
        QWidget *parent, bool *_IPv4HasDHCP, bool *_IPv6HasDHCP) :
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
    sets->addWidget(new _IPv4(this, _IPv4HasDHCP));
    sets->addWidget(new _IPv6(this, _IPv6HasDHCP));
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(manageWdg);
    commonLayout->addWidget(sets);
    setLayout(commonLayout);
    connect(ipv6, SIGNAL(toggled(bool)),
            this, SLOT(ipv6StateChanged(bool)));
    for (int i=0; i<sets->count(); i++) {
        _IPvX *wdg = static_cast<_IPvX*>(sets->widget(i));
        if ( NULL==wdg ) continue;
        connect(staticRoute, SIGNAL(toggled(bool)),
                wdg, SLOT(setStaticRouteMode(bool)));
        connect(wdg, SIGNAL(dhcpUsageChanged()),
                this, SIGNAL(dhcpUsageChanged()));
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
        if ( NULL!=wdg ) {
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
        if ( NULL!=wdg ) {
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
void _IP_Widget::updateDHCPUsage()
{
    for (int i=0; i<sets->count(); i++) {
        _IPvX *wdg = static_cast<_IPvX*>(sets->widget(i));
        if ( NULL==wdg ) continue;
        wdg->updateDHCPUsage();
    };
}

/* private slots */
void _IP_Widget::ipv6StateChanged(bool state)
{
    sets->setCurrentIndex( (state)? 1:0 );
    _IPvX *wdg = static_cast<_IPvX*>(
                sets->widget( (state)? 0:1 ));
    if ( NULL!=wdg ) wdg->setDHCPState(false);
}
