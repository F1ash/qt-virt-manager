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
    for (uint i=0; i<sets->count(); i++) {
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
    _QWidget *wdg = static_cast<_QWidget*>(
                sets->currentWidget());
    if ( NULL!=wdg ) doc = wdg->getDataDocument();
    return doc;
}
void _IP_Widget::updateDHCPUsage()
{
    for (uint i=0; i<sets->count(); i++) {
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
