#include "_changed.h"

_Changed::_Changed(QWidget *parent) :
    QWidget(parent)
{
}
_Changed::~_Changed()
{

}

/* public slots */
void _Changed::stateChanged()
{
    emit dataChanged();
}
