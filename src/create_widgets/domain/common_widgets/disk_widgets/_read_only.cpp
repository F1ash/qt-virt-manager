#include "_read_only.h"

_ReadOnly::_ReadOnly(QWidget *parent) :
    _Changed(parent)
{
    readOnly = new QCheckBox(tr("Read Only"), this);
    readOnly->setLayoutDirection(Qt::RightToLeft);
    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(readOnly);
    commonLayout->setAlignment(Qt::AlignLeft);
    setLayout(commonLayout);
    connect(readOnly, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
}

/* public slots */
bool _ReadOnly::state() const
{
    return readOnly->isChecked();
}
