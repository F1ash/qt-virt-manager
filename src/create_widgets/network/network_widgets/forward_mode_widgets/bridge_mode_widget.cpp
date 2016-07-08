#include "bridge_mode_widget.h"

BRIDGE_Mode_widget::BRIDGE_Mode_widget(QWidget *parent) :
    _QWidget(parent)
{
    info = new QLabel(
    "Forward by existing host, Open vSwitch or macvtap bridge", this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(info);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
}
