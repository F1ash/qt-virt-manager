#include "root_attributes.h"

// Rules of this type should either go
// into the root or same named chain.
#include "mac_attributes.h"
#include "vlan_attributes.h"
#include "stp_attributes.h"
#include "arp_attributes.h"
#include "rarp_attributes.h"
#include "ipv4_attributes.h"
#include "ipv6_attributes.h"

// The chain parameter is ignored for this type of traffic
// and should either be omitted or set to root.
#include "tcp_attributes.h"
#include "udp_attributes.h"
#include "sctp_attributes.h"
#include "tcp6_attributes.h"
#include "udp6_attributes.h"
#include "sctp6_attributes.h"
#include "icmp_attributes.h"
#include "icmp6_attributes.h"
#include "igmp_attributes.h"
#include "esp_attributes.h"
#include "ah_attributes.h"
#include "udplite_attributes.h"
#include "all_attributes.h"
#include "esp6_attributes.h"
#include "ah6_attributes.h"
#include "udplite6_attributes.h"
#include "all6_attributes.h"

ROOT_Attributes::ROOT_Attributes(QWidget *parent, QString tag) :
    _Attributes(parent, tag)
{
    protocolID = "root";
    attrName->addItem("MAC");
    attrEditor->addWidget(new MAC_Attributes(this));
    attrName->addItem("VLAN");
    attrEditor->addWidget(new VLAN_Attributes(this));
    attrName->addItem("STP");
    attrEditor->addWidget(new STP_Attributes(this));
    attrName->addItem("ARP");
    attrEditor->addWidget(new ARP_Attributes(this));
    attrName->addItem("RARP");
    attrEditor->addWidget(new RARP_Attributes(this));
    attrName->addItem("IPv4");
    attrEditor->addWidget(new IPv4_Attributes(this));
    attrName->addItem("IPv6");
    attrEditor->addWidget(new IPv6_Attributes(this));

    attrName->addItem("TCP");
    attrEditor->addWidget(new TCP_Attributes(this));
    attrName->addItem("UDP");
    attrEditor->addWidget(new UDP_Attributes(this));
    attrName->addItem("SCTP");
    attrEditor->addWidget(new SCTP_Attributes(this));
    attrName->addItem("TCP6");
    attrEditor->addWidget(new TCP6_Attributes(this));
    attrName->addItem("UDP6");
    attrEditor->addWidget(new UDP6_Attributes(this));
    attrName->addItem("SCTP6");
    attrEditor->addWidget(new SCTP6_Attributes(this));

    attrName->addItem("ICMP");
    attrEditor->addWidget(new ICMP_Attributes(this));
    attrName->addItem("ICMP6");
    attrEditor->addWidget(new ICMP6_Attributes(this));

    attrName->addItem("IGMP");
    attrEditor->addWidget(new IGMP_Attributes(this));
    attrName->addItem("ESP");
    attrEditor->addWidget(new ESP_Attributes(this));
    attrName->addItem("AH");
    attrEditor->addWidget(new AH_Attributes(this));
    attrName->addItem("UDPLITE");
    attrEditor->addWidget(new UDPLITE_Attributes(this));
    attrName->addItem("ALL");
    attrEditor->addWidget(new ALL_Attributes(this));
    attrName->addItem("ESP6");
    attrEditor->addWidget(new ESP6_Attributes(this));
    attrName->addItem("AH6");
    attrEditor->addWidget(new AH6_Attributes(this));
    attrName->addItem("UDPLITE6");
    attrEditor->addWidget(new UDPLITE6_Attributes(this));
    attrName->addItem("ALL6");
    attrEditor->addWidget(new ALL6_Attributes(this));

    for (uint i=0; i<attrEditor->count(); i++) {
        _Attributes *a = static_cast<_Attributes*>(
                    attrEditor->widget(i));
        if ( a==nullptr ) continue;
        connect(a, SIGNAL(dataChanged()),
                this, SLOT(dataEdited()));
        connect(a, SIGNAL(dataChanged()),
                this, SIGNAL(dataChanged()));
    };
}
void ROOT_Attributes::clearAllAttributeData()
{
    for (uint i=0; i<attrEditor->count(); i++) {
        _Attributes *a = static_cast<_Attributes*>(
                    attrEditor->widget(i));
        if ( a==nullptr ) continue;
        a->clearAllAttributeData();
    };
    attrName->setEnabled(true);
    emit released(false);
}
void ROOT_Attributes::setAttrValue(const QVariantMap &_map)
{
    int idx = -1;
    QString _protID = _map.value("protocolID").toString();
    for (int i=0; i<attrEditor->count(); i++) {
        _Attributes *e = static_cast<_Attributes*>(
                    attrEditor->widget(i));
        if ( e==nullptr ) continue;
        if ( e->getProtocolID()==_protID ) {
            idx = i;
            break;
        };
    };
    if ( -1<idx ) {
        attrName->setCurrentIndex(idx);
        attrName->setDisabled(true);
        _Attributes *a = static_cast<_Attributes*>(
                    attrEditor->currentWidget());
        if ( a!=nullptr ) a->setAttrValue(_map);
    };
}
QVariantMap ROOT_Attributes::getAttrValue(QString &attr) const
{
    QVariantMap ret;
    _Attributes *e = static_cast<_Attributes*>(
                attrEditor->currentWidget());
    if ( e!=nullptr ) {
        ret = e->getAttrValue(attr);
    };
    return ret;
}
QStringList ROOT_Attributes::getAttrList() const
{
    QStringList l;
    _Attributes *e = static_cast<_Attributes*>(
                attrEditor->currentWidget());
    if ( e!=nullptr ) {
        l = e->getAttrList();
    };
    return l;
}
void ROOT_Attributes::dataEdited()
{
    attrName->setEnabled(false);
    emit released(true);
}
