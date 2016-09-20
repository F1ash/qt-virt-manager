#include "ipv6_attributes.h"

IPv6_Attributes::IPv6_Attributes(QWidget *parent) :
    _Attributes(parent)
{
    protocolID = "ipv6";
    attrName->addItem("srcmacaddr");
    attrEditor->addWidget(new MAC_ADDR_Data(this, "srcmacaddr"));
    attrName->addItem("srcmacmask");
    attrEditor->addWidget(new MAC_MASK_Data(this, "srcmacmask"));
    attrName->addItem("dstmacaddr");
    attrEditor->addWidget(new MAC_ADDR_Data(this, "dstmacaddr"));
    attrName->addItem("dstmacmask");
    attrEditor->addWidget(new MAC_MASK_Data(this, "dstmacmask"));
    attrName->addItem("srcipaddr");
    attrEditor->addWidget(new IPv6_ADDR_Data(this, "srcipaddr"));
    attrName->addItem("srcipmask");
    attrEditor->addWidget(new IPv6_MASK_Data(this, "srcipmask"));
    attrName->addItem("dstipaddr");
    attrEditor->addWidget(new IPv6_ADDR_Data(this, "dstipaddr"));
    attrName->addItem("dstipmask");
    attrEditor->addWidget(new IPv6_MASK_Data(this, "dstipmask"));
    attrName->addItem("protocol");
    attrEditor->addWidget(new UINT8_Data(this, "protocol"));
    attrName->addItem("srcportstart");
    attrEditor->addWidget(new UINT16_Data(this, "srcportstart"));

    attrName->addItem("srcportend");
    attrEditor->addWidget(new UINT16_Data(this, "srcportend"));
    attrName->addItem("dstportstart");
    attrEditor->addWidget(new UINT16_Data(this, "dstportstart"));
    attrName->addItem("dstportend");
    attrEditor->addWidget(new UINT16_Data(this, "dstportend"));
    attrName->addItem("type");
    attrEditor->addWidget(new UINT8_Data(this, "type"));
    attrName->addItem("typeend");
    attrEditor->addWidget(new UINT8_Data(this, "typeend"));
    attrName->addItem("code");
    attrEditor->addWidget(new UINT8_Data(this, "code"));
    attrName->addItem("codeend");
    attrEditor->addWidget(new UINT8_Data(this, "codeend"));
    attrName->addItem("comment");
    attrEditor->addWidget(new STRING_Data(this, "comment"));

    for (uint i=0; i<attrEditor->count(); i++) {
        UntypedData *d = static_cast<UntypedData*>(
                    attrEditor->widget(i));
        if ( d==nullptr ) continue;
        connect(d, SIGNAL(dataChanged()),
                this, SIGNAL(dataChanged()));
    };
}
