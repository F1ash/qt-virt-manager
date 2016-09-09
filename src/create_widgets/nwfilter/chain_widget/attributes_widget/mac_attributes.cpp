#include "mac_attributes.h"

MAC_Attributes::MAC_Attributes(QWidget *parent) :
    _Attributes(parent)
{
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
}
