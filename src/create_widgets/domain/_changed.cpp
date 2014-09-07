#include "_changed.h"

_Changed::_Changed(QWidget *parent) :
    QWidget(parent)
{
}

/* private slots */
void _Changed::stateChanged()
{
    emit dataChanged();
}
