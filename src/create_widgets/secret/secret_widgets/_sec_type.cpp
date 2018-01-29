#include "_sec_type.h"

_SecType::_SecType(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    QWidget(parent), ptr_ConnPtr(connPtrPtr)
{
    info = new QLabel(this);
    baseLayout = new QVBoxLayout(this);
    baseLayout->addWidget(info);
    setLayout(baseLayout);
}

QDomDocument _SecType::getSecStuff() const
{
    return QDomDocument();
}
