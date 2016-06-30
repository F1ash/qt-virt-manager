#ifndef QOS_WIDGET_H
#define QOS_WIDGET_H

#include "_checked_widget.h"

class QoS_Widget : public _Checked_Widget
{
    Q_OBJECT
public:
    explicit QoS_Widget(
            QWidget *parent = nullptr,
            QString  tag = "QoS");

signals:

public slots:
    QDomDocument     getDataDocument() const;
};

#endif // QOS_WIDGET_H
