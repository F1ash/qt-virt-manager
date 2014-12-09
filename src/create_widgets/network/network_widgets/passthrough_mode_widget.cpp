#include "passthrough_mode_widget.h"

PASSTHROUGH_Mode_widget::PASSTHROUGH_Mode_widget(QWidget *parent) :
    QWidget(parent)
{
    commonLayout = new QVBoxLayout(this);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
}
