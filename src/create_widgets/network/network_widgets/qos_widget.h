#ifndef QOS_WIDGET_H
#define QOS_WIDGET_H

#include "create_widgets/common/_checked_widget.h"
#include "qos_widgets/bound_widget.h"
#include <QTabWidget>

class QoS_Widget : public _Checked_Widget
{
    Q_OBJECT
public:
    explicit QoS_Widget(
            QWidget *parent = Q_NULLPTR,
            QString  tag = "QoS");

signals:

private:
    Bound_Widget    *inbound, *outbound;
    QTabWidget      *bandWidth;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
};

#endif // QOS_WIDGET_H
