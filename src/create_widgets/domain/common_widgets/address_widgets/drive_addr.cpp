#include "drive_addr.h"

DriveAddr::DriveAddr(QWidget *parent) :
    _Addr(parent)
{
    contrLabel = new QLabel("Controller:", this);
    busLabel = new QLabel("Bus:", this);
    targetLabel = new QLabel("Target:", this);
    unitLabel = new QLabel("Unit:", this);
    controller = new QLineEdit(this);
    controller->setPlaceholderText("a 2-digit controller number");
    bus = new QLineEdit(this);
    bus->setPlaceholderText("a 2-digit bus number");
    target = new QLineEdit(this);
    target->setPlaceholderText("a 2-digit bus number");
    unit = new QLineEdit(this);
    unit->setPlaceholderText("a 2-digit unit number on the bus");
    commonlayout = new QGridLayout();
    commonlayout->addWidget(contrLabel, 0, 0);
    commonlayout->addWidget(busLabel, 1, 0);
    commonlayout->addWidget(targetLabel, 2, 0);
    commonlayout->addWidget(unitLabel, 3, 0);
    commonlayout->addWidget(controller, 0, 1);
    commonlayout->addWidget(bus, 1, 1);
    commonlayout->addWidget(target, 2, 1);
    commonlayout->addWidget(unit, 3, 1);
    setLayout(commonlayout);
    connect(controller, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(bus, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(target, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(unit, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
}
AttrList DriveAddr::getAttrList() const
{
    AttrList attrs;
    if ( !controller->text().isEmpty() )
        attrs.insert("controller", controller->text());
    if ( !bus->text().isEmpty() )
        attrs.insert("bus", bus->text());
    if ( !target->text().isEmpty() )
        attrs.insert("target", target->text());
    if ( !unit->text().isEmpty() )
        attrs.insert("unit", unit->text());
    return attrs;
}
