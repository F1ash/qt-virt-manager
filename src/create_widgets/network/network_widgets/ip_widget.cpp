#include "ip_widget.h"

IP_Widget::IP_Widget(QWidget *parent, QString tag) :
    _Checked_Widget(parent, tag)
{
    ipv6 = new QCheckBox("Use IPv6", this);
    sets = new QStackedWidget(this);
    sets->addWidget(new _IPv4(this));
    sets->addWidget(new _IPv6(this));
    baseLayout->addWidget(ipv6);
    baseLayout->addWidget(sets);
    connect(ipv6, SIGNAL(toggled(bool)),
            this, SLOT(ipv6StateChanged(bool)));
}

/* public slots */
QDomDocument IP_Widget::getDataDocument() const
{
    QDomDocument doc;
    _QWidget *wdg = static_cast<_QWidget*>(
                sets->currentWidget());
    if ( NULL!=wdg ) doc = wdg->getDataDocument();
    return doc;
}
void IP_Widget::ipv6StateChanged(bool state)
{
    sets->setCurrentIndex( (state)? 1:0 );
}
