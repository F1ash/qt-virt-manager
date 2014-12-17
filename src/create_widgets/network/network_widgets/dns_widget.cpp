#include "dns_widget.h"

DNS_Widget::DNS_Widget(QWidget *parent, QString tag) :
    _Checked_Widget(parent, tag)
{

}

/* public slots */
QDomDocument DNS_Widget::getDataDocument() const
{
    QDomDocument doc;
    return doc;
}
