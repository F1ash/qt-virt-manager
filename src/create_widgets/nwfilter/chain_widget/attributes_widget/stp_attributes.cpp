#include "stp_attributes.h"

STP_Attributes::STP_Attributes(QWidget *parent) :
    _Attributes(parent)
{
    protocolID = "stp";
    attrName->addItem("srcmacaddr");
    attrEditor->addWidget(new MAC_ADDR_Data(this, "srcmacaddr"));
    attrName->addItem("srcmacmask");
    attrEditor->addWidget(new MAC_MASK_Data(this, "srcmacmask"));
    attrName->addItem("type");
    attrEditor->addWidget(new UINT8_Data(this, "type"));
    attrName->addItem("flags");
    attrEditor->addWidget(new UINT8_Data(this, "flags"));
    attrName->addItem("root-priority");
    attrEditor->addWidget(new UINT16_Data(this, "root-priority"));
    attrName->addItem("root-priority-hi");
    attrEditor->addWidget(new UINT16_Data(this, "root-priority-hi"));
    attrName->addItem("root-address");
    attrEditor->addWidget(new MAC_ADDR_Data(this, "root-address"));
    attrName->addItem("root-address-mask");
    attrEditor->addWidget(new MAC_MASK_Data(this, "root-address-mask"));
    attrName->addItem("root-cost");
    attrEditor->addWidget(new UINT32_Data(this, "root-cost"));
    attrName->addItem("root-cost-hi");
    attrEditor->addWidget(new UINT32_Data(this, "root-cost-hi"));

    attrName->addItem("sender-priority");
    attrEditor->addWidget(new UINT16_Data(this, "sender-priority"));
    attrName->addItem("sender-priority-hi");
    attrEditor->addWidget(new UINT16_Data(this, "sender-priority-hi"));
    attrName->addItem("sender-address");
    attrEditor->addWidget(new MAC_ADDR_Data(this, "sender-address"));
    attrName->addItem("sender-address-mask");
    attrEditor->addWidget(new MAC_MASK_Data(this, "sender-address-mask"));
    attrName->addItem("port");
    attrEditor->addWidget(new UINT16_Data(this, "port"));
    attrName->addItem("port_hi");
    attrEditor->addWidget(new UINT16_Data(this, "port_hi"));
    attrName->addItem("msg-age");
    attrEditor->addWidget(new UINT16_Data(this, "msg-age"));
    attrName->addItem("msg-age-hi");
    attrEditor->addWidget(new UINT16_Data(this, "msg-age-hi"));
    attrName->addItem("max-age");
    attrEditor->addWidget(new UINT16_Data(this, "max-age"));
    attrName->addItem("max-age-hi");
    attrEditor->addWidget(new UINT16_Data(this, "max-age-hi"));

    attrName->addItem("hello-time");
    attrEditor->addWidget(new UINT16_Data(this, "hello-time"));
    attrName->addItem("hello-time-hi");
    attrEditor->addWidget(new UINT16_Data(this, "hello-time-hi"));
    attrName->addItem("forward-delay");
    attrEditor->addWidget(new UINT16_Data(this, "forward-delay"));
    attrName->addItem("forward-delay-hi");
    attrEditor->addWidget(new UINT16_Data(this, "forward-delay-hi"));
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
