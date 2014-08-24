#include "isa_addr.h"

IsaAddr::IsaAddr(QWidget *parent) :
    _Addr(parent)
{
    iobaseLabel = new QLabel("IOBase:", this);
    irqLabel = new QLabel("IRC:", this);
    iobase = new QLineEdit(this);
    irq = new QLineEdit(this);
    commonlayout = new QGridLayout();
    commonlayout->addWidget(iobaseLabel, 0, 0);
    commonlayout->addWidget(irqLabel, 1, 0);
    commonlayout->addWidget(iobase, 0, 1);
    commonlayout->addWidget(irq, 1, 1);
    setLayout(commonlayout);
}
AttrList IsaAddr::getAttrList() const
{
    AttrList attrs;
    if ( !iobase->text().isEmpty() )
        attrs.insert("iobase", iobase->text());
    if ( !irq->text().isEmpty() )
        attrs.insert("irq", irq->text());
    return attrs;
}
