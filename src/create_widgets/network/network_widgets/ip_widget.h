#ifndef IP_WIDGET_H
#define IP_WIDGET_H

#include "_checked_widget.h"
#include <QStackedWidget>
#include "_ipv4.h"
#include "_ipv6.h"

class IP_Widget : public _Checked_Widget
{
    Q_OBJECT
public:
    IP_Widget(
            QWidget *parent = NULL,
            QString  tag = "IP");

private:
    QCheckBox       *ipv6;
    QStackedWidget  *sets;
    _IPv4           *_ipv4;
    _IPv6           *_ipv6;

public slots:
    QDomDocument     getDataDocument() const;
    void             ipv6StateChanged(bool);
};

#endif // IP_WIDGET_H
