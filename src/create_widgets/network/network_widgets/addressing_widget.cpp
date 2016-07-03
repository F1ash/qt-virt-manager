#include "addressing_widget.h"

Addressing_Widget::Addressing_Widget(QWidget *parent, QString tag) :
    _Checked_Widget(parent, tag)
{
    dns = new DNS_Widget(this);
    ip = new IP_Widget(this);
    mac = new MAC_Widget(this);
    baseLayout->addWidget(dns);
    baseLayout->addWidget(ip);
    baseLayout->addWidget(mac);
    baseLayout->addStretch(-1);
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
void Addressing_Widget::setDataDescription(QString &_xmlDesc)
{
    dns->setDataDescription(_xmlDesc);
    ip->setDataDescription(_xmlDesc);
    mac->setDataDescription(_xmlDesc);
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
