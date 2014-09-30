#include "ccid_addr.h"

CCIDAddr::CCIDAddr(QWidget *parent) :
    _Addr(parent)
{
    busLabel = new QLabel("Bus:", this);
    slotLabel = new QLabel("Slot:", this);
    bus = new QLineEdit(this);
    bus->setPlaceholderText("a 2-digit bus number");
    slot = new QLineEdit(this);
    slot->setPlaceholderText("a 2-digit slot within the bus");
    commonlayout = new QGridLayout();
    commonlayout->addWidget(busLabel, 0, 0);
    commonlayout->addWidget(slotLabel, 1, 0);
    commonlayout->addWidget(bus, 0, 1);
    commonlayout->addWidget(slot, 1, 1);
    setLayout(commonlayout);
    connect(bus, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(slot, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
}
AttrList CCIDAddr::getAttrList() const
{
    AttrList attrs;
    if ( !bus->text().isEmpty() )
        attrs.insert("bus", bus->text());
    if ( !slot->text().isEmpty() )
        attrs.insert("slot", slot->text());
    return attrs;
}
