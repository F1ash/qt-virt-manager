#include "qos_widget.h"

QoS_Widget::QoS_Widget(
        QWidget *parent, QString tag) :
    _Checked_Widget(parent, tag)
{
    // WARNING: Currently, the Linux kernel doesn't allow
    // ingress qdiscs to have any classes therefore floor
    // can be applied only on inbound and not outbound.
    inbound = new Bound_Widget(this);
    inbound->setFloorUsage(true);
    outbound = new Bound_Widget(this);
    outbound->setFloorUsage(false);
    bandWidth = new QTabWidget(this);
    bandWidth->addTab(inbound, "Inbound");
    bandWidth->addTab(outbound, "Outbound");
    baseLayout->addWidget(bandWidth);
}

/* public slots */
QDomDocument QoS_Widget::getDataDocument() const
{
    QDomDocument doc;
    return doc;
}
