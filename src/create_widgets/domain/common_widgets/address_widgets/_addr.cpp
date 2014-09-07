#include "_addr.h"

_Addr::_Addr(QWidget *parent) :
    _Changed(parent)
{
}

/* public slots */
AttrList _Addr::getAttrList() const
{
    return AttrList();
}
