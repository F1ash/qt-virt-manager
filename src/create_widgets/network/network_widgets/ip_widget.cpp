#include "ip_widget.h"

IP_Widget::IP_Widget(QWidget *parent, QString tag) :
    _Checked_Widget(parent, tag)
{

}

/* public slots */
QDomDocument IP_Widget::getDataDocument() const
{
    QDomDocument doc;
    return doc;
}
