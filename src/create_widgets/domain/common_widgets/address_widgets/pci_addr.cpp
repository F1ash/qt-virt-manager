#include "pci_addr.h"

PciAddr::PciAddr(QWidget *parent) :
    _Addr(parent)
{
    domainLabel = new QLabel("Domain:", this);
    busLabel = new QLabel("Bus:", this);
    slotLabel = new QLabel("Slot:", this);
    functLabel = new QLabel("Function:", this);
    domain = new QLineEdit(this);
    domain->setPlaceholderText("a 2-byte hex integer, not currently used by qemu");
    bus = new QLineEdit(this);
    bus->setPlaceholderText("a hex value between 0 and 0xff, inclusive");
    slot = new QLineEdit(this);
    slot->setPlaceholderText("a hex value between 0x0 and 0x1f, inclusive");
    function = new QSpinBox(this);
    function->setRange(0, 7);
    commonlayout = new QGridLayout();
    commonlayout->addWidget(domainLabel, 0, 0);
    commonlayout->addWidget(busLabel, 1, 0);
    commonlayout->addWidget(slotLabel, 2, 0);
    commonlayout->addWidget(functLabel, 3, 0);
    commonlayout->addWidget(domain, 0, 1);
    commonlayout->addWidget(bus, 1, 1);
    commonlayout->addWidget(slot, 2, 1);
    commonlayout->addWidget(function, 3, 1);
    setLayout(commonlayout);
    connect(domain, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(bus, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(slot, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(function, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
}
AttrList PciAddr::getAttrList() const
{
    AttrList attrs;
    if ( !domain->text().isEmpty() )
        attrs.insert("domain", domain->text());
    if ( !bus->text().isEmpty() )
        attrs.insert("bus", bus->text());
    if ( !slot->text().isEmpty() )
        attrs.insert("slot", slot->text());
    if ( !function->text().isEmpty() )
        attrs.insert("function", function->text());
    return attrs;
}
