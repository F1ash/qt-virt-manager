#include "rarp_attributes.h"

RARP_Attributes::RARP_Attributes(QWidget *parent) :
    _Attributes(parent)
{
    protocolID = "rarp";
    attrName->addItem("srcmacaddr");
    attrEditor->addWidget(new MAC_ADDR_Data(this, "srcmacaddr"));
    attrName->addItem("srcmacmask");
    attrEditor->addWidget(new MAC_MASK_Data(this, "srcmacmask"));
    attrName->addItem("dstmacaddr");
    attrEditor->addWidget(new MAC_ADDR_Data(this, "dstmacaddr"));
    attrName->addItem("dstmacmask");
    attrEditor->addWidget(new MAC_MASK_Data(this, "dstmacmask"));
    attrName->addItem("hwtype");
    attrEditor->addWidget(new UINT16_Data(this, "hwtype"));
    attrName->addItem("protocoltype");
    attrEditor->addWidget(new UINT16_Data(this, "protocoltype"));
    attrName->addItem("opcode");
    attrEditor->addWidget(new STRING_Data(this, "opcode"));
    attrName->addItem("arpsrcmacaddr");
    attrEditor->addWidget(new MAC_ADDR_Data(this, "arpsrcmacaddr"));
    attrName->addItem("arpdstmacaddr");
    attrEditor->addWidget(new MAC_ADDR_Data(this, "arpdstmacaddr"));
    attrName->addItem("arpsrcipaddr");
    attrEditor->addWidget(new IP_ADDR_Data(this, "arpsrcipaddr"));
    attrName->addItem("arpsrcipmask");
    attrEditor->addWidget(new IP_MASK_Data(this, "arpsrcipmask"));
    attrName->addItem("arpdstipaddr");
    attrEditor->addWidget(new IP_ADDR_Data(this, "arpdstipaddr"));
    attrName->addItem("arpdstipmask");
    attrEditor->addWidget(new IP_MASK_Data(this, "arpdstipmask"));
    attrName->addItem("comment");
    attrEditor->addWidget(new STRING_Data(this, "comment"));
    attrName->addItem("gratuitous");
    attrEditor->addWidget(new BOOLEAN_Data(this, "gratuitous"));

    for (uint i=0; i<attrEditor->count(); i++) {
        UntypedData *d = static_cast<UntypedData*>(
                    attrEditor->widget(i));
        if ( d==nullptr ) continue;
        connect(d, SIGNAL(dataChanged()),
                this, SIGNAL(dataChanged()));
    };
}
