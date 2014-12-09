#include "bridge_mode_widget.h"

BRIDGE_Mode_widget::BRIDGE_Mode_widget(QWidget *parent) :
    QWidget(parent)
{
    commonLayout = new QVBoxLayout(this);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
}
