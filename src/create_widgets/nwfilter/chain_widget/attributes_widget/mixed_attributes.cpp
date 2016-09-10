#include "mixed_attributes.h"
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

MIXED_Attributes::MIXED_Attributes(QWidget *parent, QString tag) :
    _Attributes(parent, tag)
{
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
    };
}
void MIXED_Attributes::clearAllAttributeData()
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
void MIXED_Attributes::dataEdited()
{
    attrName->setEnabled(false);
    emit released(true);
}
