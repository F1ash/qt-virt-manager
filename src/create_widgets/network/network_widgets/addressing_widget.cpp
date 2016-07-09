#include "addressing_widget.h"

Addressing_Widget::Addressing_Widget(QWidget *parent, QString tag) :
    _Checked_Widget(parent, tag)
{
    dns = new DNS_Widget(this);
    ip = new IP_Widget(this);
    mac = new MAC_Widget(this);
    scrollLayout = new QVBoxLayout(this);
    scrollLayout->addWidget(dns);
    scrollLayout->addWidget(ip);
    scrollLayout->addWidget(mac);
    scrollLayout->addStretch(-1);
    scrolled = new QWidget(this);
    scrolled->setLayout(scrollLayout);
    scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    baseLayout->addWidget(scroll);
    baseLayout->addStretch(-1);
    // set scroll widget after filling all widgets
    // for correct displaying
}

/* public slots */
QDomDocument Addressing_Widget::getDataDocument() const
{
    QDomDocument doc;
    doc.setContent(QString());
    if ( dns->isUsed() )
        doc.appendChild(dns->getDataDocument());
    if ( ip->isUsed() )
        doc.appendChild(ip->getDataDocument());
    if ( mac->isUsed() )
        doc.appendChild(mac->getDataDocument());
    return doc;
}
void Addressing_Widget::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _network, _dns, _ip, _mac;
    _network = doc.firstChildElement("network");
    _dns = _network.firstChildElement("dns");
    _ip = _network.firstChildElement("ip");
    _mac = _network.firstChildElement("mac");
    if ( !_network.isNull() ) {
        if ( !_dns.isNull() ) {
            setUsage(true);
            dns->setDataDescription(_xmlDesc);
        };
        if ( !_ip.isNull() ) {
            setUsage(true);
            ip->setDataDescription(_xmlDesc);
        };
        if ( !_mac.isNull() ) {
            setUsage(true);
            mac->setDataDescription(_xmlDesc);
        };
    };
    // set scroll widget for correct displaying
    scroll->setWidget(scrolled);
}
void Addressing_Widget::ipv6Changed(bool state)
{
    this->setUsage(state);
    this->setFreez(state);
    dns->setUsage(false);
    ip->setUsage(false);
    mac->setUsage(state);
    dns->setDisabled(state);
    ip->setDisabled(state);
}
