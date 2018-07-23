#include "ipv4_attributes.h"

IPv4_Attributes::IPv4_Attributes(QWidget *parent) :
    _Attributes(parent)
{
    protocolID = "ip";
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
    attrName->addItem("protocol");
    attrEditor->addWidget(new STRING_Data(this, "protocol"));
    attrName->addItem("srcportstart");
    attrEditor->addWidget(new UINT16_Data(this, "srcportstart"));

    attrName->addItem("srcportend");
    attrEditor->addWidget(new UINT16_Data(this, "srcportend"));
    attrName->addItem("dstportstart");
    attrEditor->addWidget(new UINT16_Data(this, "dstportstart"));
    attrName->addItem("dstportend");
    attrEditor->addWidget(new UINT16_Data(this, "dstportend"));
    attrName->addItem("dscp");
    attrEditor->addWidget(new UINT8_Data(this, "dscp", "63"));
    attrName->addItem("comment");
    attrEditor->addWidget(new STRING_Data(this, "comment"));

    for (int i=0; i<attrEditor->count(); i++) {
        UntypedData *d = static_cast<UntypedData*>(
                    attrEditor->widget(i));
        if ( d==Q_NULLPTR ) continue;
        connect(d, SIGNAL(dataChanged()),
                this, SIGNAL(dataChanged()));
    };
}
