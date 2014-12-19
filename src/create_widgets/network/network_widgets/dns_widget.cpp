#include "dns_widget.h"

DNS_Widget::DNS_Widget(QWidget *parent, QString tag) :
    _Checked_Widget(parent, tag)
{
    forwardPlainNames = new QCheckBox("forwardPlainNames", this);
    forwarder = new Forwarder(this);
    baseLayout->addWidget(forwardPlainNames);
    baseLayout->addWidget(forwarder);
}

/* public slots */
QDomDocument DNS_Widget::getDataDocument() const
{
    QDomDocument doc;
    return doc;
}
