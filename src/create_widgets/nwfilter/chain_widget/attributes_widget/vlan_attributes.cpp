#include "vlan_attributes.h"

VLAN_Attributes::VLAN_Attributes(QWidget *parent) :
    _Attributes(parent)
{
    protocolID = "vlan";
    attrName->addItem("srcmacaddr");
    attrEditor->addWidget(new MAC_ADDR_Data(this, "srcmacaddr"));
    attrName->addItem("srcmacmask");
    attrEditor->addWidget(new MAC_MASK_Data(this, "srcmacmask"));
    attrName->addItem("dstmacaddr");
    attrEditor->addWidget(new MAC_ADDR_Data(this, "dstmacaddr"));
    attrName->addItem("dstmacmask");
    attrEditor->addWidget(new MAC_MASK_Data(this, "dstmacmask"));
    attrName->addItem("vlanid");
    attrEditor->addWidget(new UINT16_Data(this, "vlanid", "4095"));
    attrName->addItem("encap-protocol");
    attrEditor->addWidget(new STRING_Data(this, "encap-protocol"));
    attrName->addItem("comment");
    attrEditor->addWidget(new STRING_Data(this, "comment"));

    for (int i=0; i<attrEditor->count(); i++) {
        UntypedData *d = static_cast<UntypedData*>(
                    attrEditor->widget(i));
        if ( d==nullptr ) continue;
        connect(d, SIGNAL(dataChanged()),
                this, SIGNAL(dataChanged()));
    };
}
