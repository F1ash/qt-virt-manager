#include "mac_attributes.h"

MAC_Attributes::MAC_Attributes(QWidget *parent) :
    _Attributes(parent)
{
    protocolID = "mac";
    attrName->addItem("srcmacaddr");
    attrEditor->addWidget(new MAC_ADDR_Data(this, "srcmacaddr"));
    attrName->addItem("srcmacmask");
    attrEditor->addWidget(new MAC_MASK_Data(this, "srcmacmask"));
    attrName->addItem("dstmacaddr");
    attrEditor->addWidget(new MAC_ADDR_Data(this, "dstmacaddr"));
    attrName->addItem("dstmacmask");
    attrEditor->addWidget(new MAC_MASK_Data(this, "dstmacmask"));
    attrName->addItem("protocolid");
    attrEditor->addWidget(new STRING_Data(this, "protocolid"));
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
