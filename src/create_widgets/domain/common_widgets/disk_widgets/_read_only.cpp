#include "_read_only.h"

_ReadOnly::_ReadOnly(QWidget *parent) :
    QWidget(parent)
{
    readOnly = new QCheckBox("Read Only", this);
    readOnly->setLayoutDirection(Qt::RightToLeft);
    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(readOnly);
    commonLayout->setAlignment(Qt::AlignLeft);
    setLayout(commonLayout);
}

/* public slots */
bool _ReadOnly::state() const
{
    return readOnly->isChecked();
}
