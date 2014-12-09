#include "private_mode_widget.h"

PRIVATE_Mode_widget::PRIVATE_Mode_widget(QWidget *parent) :
    QWidget(parent)
{
    commonLayout = new QVBoxLayout(this);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
}
