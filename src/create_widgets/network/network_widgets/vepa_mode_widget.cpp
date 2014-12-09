#include "vepa_mode_widget.h"

VEPA_Mode_widget::VEPA_Mode_widget(QWidget *parent) :
    QWidget(parent)
{
    commonLayout = new QVBoxLayout(this);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
}
