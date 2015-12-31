#include "_changed.h"

_Changed::_Changed(QWidget *parent) :
    QWidget(parent)
{
}
_Changed::~_Changed()
{

}

/* private slots */
void _Changed::stateChanged()
{
    emit dataChanged();
}
