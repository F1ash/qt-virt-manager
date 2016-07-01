#ifndef QOS_WIDGET_H
#define QOS_WIDGET_H

#include "_checked_widget.h"
#include "qos_widgets/bound_widget.h"
#include <QTabWidget>

class QoS_Widget : public _Checked_Widget
{
    Q_OBJECT
public:
    explicit QoS_Widget(
            QWidget *parent = nullptr,
            QString  tag = "QoS");
    Bound_Widget    *inbound, *outbound;

signals:

private:
    QTabWidget      *bandWidth;

public slots:
    QDomDocument     getDataDocument() const;
};

#endif // QOS_WIDGET_H
