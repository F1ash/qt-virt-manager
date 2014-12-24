#include "route_mode_widget.h"

ROUTE_Mode_widget::ROUTE_Mode_widget(QWidget *parent) :
    _QWidget(parent)
{
    commonLayout = new QVBoxLayout(this);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
}

/* public slots */
QDomDocument ROUTE_Mode_widget::getDataDocument() const
{
    QDomDocument doc;
    return doc;
}
