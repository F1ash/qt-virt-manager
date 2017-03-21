#include "igmp_attributes.h"

IGMP_Attributes::IGMP_Attributes(QWidget *parent) :
    _Attributes(parent)
{
    protocolID = "igmp";
    attrName->addItem("srcmacaddr");
    attrEditor->addWidget(new MAC_ADDR_Data(this, "srcmacaddr"));
    attrName->addItem("srcmacmask");
    attrEditor->addWidget(new MAC_MASK_Data(this, "srcmacmask"));
    attrName->addItem("dstmacaddr");
    attrEditor->addWidget(new MAC_ADDR_Data(this, "dstmacaddr"));
    attrName->addItem("dstmacmask");
    attrEditor->addWidget(new MAC_MASK_Data(this, "dstmacmask"));
    attrName->addItem("srcipaddr");
    attrEditor->addWidget(new IP_ADDR_Data(this, "srcipaddr"));
    attrName->addItem("srcipmask");
    attrEditor->addWidget(new IP_MASK_Data(this, "srcipmask"));
    attrName->addItem("dstipaddr");
    attrEditor->addWidget(new IP_ADDR_Data(this, "dstipaddr"));
    attrName->addItem("dstipmask");
    attrEditor->addWidget(new IP_MASK_Data(this, "dstipmask"));
    attrName->addItem("srcipfrom");
    attrEditor->addWidget(new IP_ADDR_Data(this, "srcipfrom"));
    attrName->addItem("srcipto");
    attrEditor->addWidget(new IP_ADDR_Data(this, "srcipto"));

    attrName->addItem("dstipfrom");
    attrEditor->addWidget(new IP_ADDR_Data(this, "dstipfrom"));
    attrName->addItem("dstipto");
    attrEditor->addWidget(new IP_ADDR_Data(this, "dstipto"));
    attrName->addItem("dscp");
    attrEditor->addWidget(new UINT8_Data(this, "dscp", "63"));
    attrName->addItem("comment");
    attrEditor->addWidget(new STRING_Data(this, "comment"));
    attrName->addItem("state");
    attrEditor->addWidget(new STRING_Data(this, "state"));
    attrName->addItem("ipset");
    attrEditor->addWidget(new STRING_Data(this, "ipset"));
    attrName->addItem("ipsetflags");
    attrEditor->addWidget(new IPSETFLAGS_Data(this, "ipsetflags"));

    for (int i=0; i<attrEditor->count(); i++) {
        UntypedData *d = static_cast<UntypedData*>(
                    attrEditor->widget(i));
        if ( d==nullptr ) continue;
        connect(d, SIGNAL(dataChanged()),
                this, SIGNAL(dataChanged()));
    };
}
