#ifndef _IP_WIDGET_H
#define _IP_WIDGET_H

#include "create_widgets/common/_qwidget.h"
#include <QStackedWidget>
#include "_ipv4.h"
#include "_ipv6.h"

class _IP_Widget : public _QWidget
{
    Q_OBJECT
public:
    explicit _IP_Widget(
            QWidget *parent       = Q_NULLPTR,
            bool    _IPv4HasDHCP = false,
            bool    _IPv6HasDHCP = false);
    QCheckBox       *ipv6, *staticRoute;
    _IPv4           *_ipv4;
    _IPv6           *_ipv6;
    void             setTabIdx(int);
    void             tabToClose();

signals:
    void             dhcpUsageChanged(uint, uint, bool);

private:
    int              tabIdx = -1;
    QHBoxLayout     *manageLayout;
    QWidget         *manageWdg;
    QStackedWidget  *sets;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
    void             updateDHCPUsage(uint, bool);

private slots:
    void             ipv6StateChanged(bool);
    void             dhcpUsageChanged(uint, bool);
    void             staticRouteChanged(bool);
};

#endif // _IP_WIDGET_H
