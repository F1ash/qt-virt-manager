#ifndef _IP_WIDGET_H
#define _IP_WIDGET_H

#include "create_widgets/domain/_qwidget.h"
#include <QStackedWidget>
#include "_ipv4.h"
#include "_ipv6.h"

class _IP_Widget : public _QWidget
{
    Q_OBJECT
public:
    explicit _IP_Widget(
            QWidget *parent = NULL,
            bool    *_IPv4HasDHCP = NULL,
            bool    *_IPv6HasDHCP = NULL);
    QCheckBox       *ipv6, *staticRoute;
    _IPv4           *_ipv4;
    _IPv6           *_ipv6;

signals:
    void             dhcpUsageChanged();

private:
    QHBoxLayout     *manageLayout;
    QWidget         *manageWdg;
    QStackedWidget  *sets;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
    void             updateDHCPUsage();

private slots:
    void             ipv6StateChanged(bool);
};

#endif // _IP_WIDGET_H
