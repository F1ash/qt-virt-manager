#include "virtioserial_addr.h"

VirtioSerialAddr::VirtioSerialAddr(QWidget *parent) :
    _Addr(parent)
{
    contrLabel = new QLabel("Controller:", this);
    busLabel = new QLabel("Bus:", this);
    slotLabel = new QLabel("Slot:", this);
    controller = new QLineEdit(this);
    controller->setPlaceholderText(
                tr("a 2-digit controller number"));
    bus = new QLineEdit(this);
    bus->setPlaceholderText(
                tr("a 2-digit bus number"));
    slot = new QLineEdit(this);
    slot->setPlaceholderText(
                tr("a 2-digit slot within the bus"));
    commonlayout = new QGridLayout();
    commonlayout->addWidget(contrLabel, 0, 0);
    commonlayout->addWidget(busLabel, 1, 0);
    commonlayout->addWidget(slotLabel, 2, 0);
    commonlayout->addWidget(controller, 0, 1);
    commonlayout->addWidget(bus, 1, 1);
    commonlayout->addWidget(slot, 2, 1);
    setLayout(commonlayout);
    connect(controller, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(bus, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(slot, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
}
AttrList VirtioSerialAddr::getAttrList() const
{
    AttrList attrs;
    if ( !controller->text().isEmpty() )
        attrs.insert("controller", controller->text());
    if ( !bus->text().isEmpty() )
        attrs.insert("bus", bus->text());
    if ( !slot->text().isEmpty() )
        attrs.insert("slot", slot->text());
    return attrs;
}
