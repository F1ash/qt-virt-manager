#include "hostdev_mode_widget.h"

HOSTDEV_Mode_widget::HOSTDEV_Mode_widget(QWidget *parent) :
    QWidget(parent)
{
    commonLayout = new QVBoxLayout(this);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
}
