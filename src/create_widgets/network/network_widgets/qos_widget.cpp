#include "qos_widget.h"

QoS_Widget::QoS_Widget(
        QWidget *parent, QString tag) :
    _Checked_Widget(parent, tag)
{

}

/* public slots */
QDomDocument QoS_Widget::getDataDocument() const
{
    QDomDocument doc;
    return doc;
}
