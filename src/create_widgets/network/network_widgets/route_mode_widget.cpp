#include "route_mode_widget.h"

ROUTE_Mode_widget::ROUTE_Mode_widget(QWidget *parent) :
    QWidget(parent)
{
    devLabel = new QLabel("Dev name:", this);
    dev = new QLineEdit(this);
    devLayout = new QHBoxLayout(this);
    devLayout->addWidget(devLabel);
    devLayout->addWidget(dev);
    devWdg = new QWidget(this);
    devWdg->setLayout(devLayout);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(devWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
}
