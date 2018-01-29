#include "usb_addr.h"

USBAddr::USBAddr(QWidget *parent) :
    _Addr(parent)
{
    busLabel = new QLabel("Bus:", this);
    portLabel = new QLabel("Port:", this);
    bus = new QLineEdit(this);
    bus->setPlaceholderText(
                tr("a hex value between 0 and 0xfff, inclusive"));
    port = new QLineEdit(this);
    port->setPlaceholderText(
                tr("a dotted notation, such as 1.2 or 2.1.3.1"));
    commonlayout = new QGridLayout();
    commonlayout->addWidget(busLabel, 0, 0);
    commonlayout->addWidget(portLabel, 1, 0);
    commonlayout->addWidget(bus, 0, 1);
    commonlayout->addWidget(port, 1, 1);
    setLayout(commonlayout);
    connect(bus, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(port, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
}
AttrList USBAddr::getAttrList() const
{
    AttrList attrs;
    if ( !bus->text().isEmpty() )
        attrs.insert("bus", bus->text());
    if ( !port->text().isEmpty() )
        attrs.insert("port", port->text());
    return attrs;
}
