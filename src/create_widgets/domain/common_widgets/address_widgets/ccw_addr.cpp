#include "ccw_addr.h"

CCWAddr::CCWAddr(QWidget *parent) :
    _Addr(parent)
{
    machineLabel = new QLabel("Controller:", this);
    cssidLabel = new QLabel("Bus:", this);
    ssidLabel = new QLabel("Target:", this);
    devnoLabel = new QLabel("Unit:", this);
    machine = new QLineEdit(this);
    machine->setText("s390-ccw-virtio");
    machine->setReadOnly(true);
    cssid = new QLineEdit(this);
    cssid->setPlaceholderText("a hex value between 0 and 0xfe, inclusive");
    ssid = new QSpinBox(this);
    ssid->setRange(0, 3);
    devno = new QLineEdit(this);
    devno->setPlaceholderText("a hex value between 0 and 0xffff, inclusive");
    commonlayout = new QGridLayout();
    commonlayout->addWidget(machineLabel, 0, 0);
    commonlayout->addWidget(cssidLabel, 1, 0);
    commonlayout->addWidget(ssidLabel, 2, 0);
    commonlayout->addWidget(devnoLabel, 3, 0);
    commonlayout->addWidget(machine, 0, 1);
    commonlayout->addWidget(cssid, 1, 1);
    commonlayout->addWidget(ssid, 2, 1);
    commonlayout->addWidget(devno, 3, 1);
    setLayout(commonlayout);
}
AttrList CCWAddr::getAttrList() const
{
    AttrList attrs;
    if ( !machine->text().isEmpty() )
        attrs.insert("machine", machine->text());
    if ( !cssid->text().isEmpty() )
        attrs.insert("cssid", cssid->text());
    attrs.insert("ssid", ssid->text());
    if ( !devno->text().isEmpty() )
        attrs.insert("devno", devno->text());
    return attrs;
}
