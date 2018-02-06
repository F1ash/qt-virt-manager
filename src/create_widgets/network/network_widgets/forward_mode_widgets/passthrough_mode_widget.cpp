#include "passthrough_mode_widget.h"

PASSTHROUGH_Mode_widget::PASSTHROUGH_Mode_widget(QWidget *parent) :
    _QWidget(parent)
{
    info = new QLabel(
tr("<font color='red'><b>Don't implemented for edit</b></font>"),
                this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(info);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
}
