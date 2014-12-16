#include "addressing_widget.h"

Addressing_Widget::Addressing_Widget(QWidget *parent) :
    _QWidget(parent)
{
    usage = new QCheckBox("Addressing", this);
    baseLayout = new QVBoxLayout(this);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    baseWdg->setVisible(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(usage);
    commonLayout->addWidget(baseWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(usage, SIGNAL(toggled(bool)),
            baseWdg, SLOT(setVisible(bool)));
}

/* public slots */
bool Addressing_Widget::isUsed() const
{
    return usage->isChecked();
}
void Addressing_Widget::setUsage(bool state)
{
    usage->setChecked(state);
}
