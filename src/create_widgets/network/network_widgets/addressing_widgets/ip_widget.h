#ifndef IP_WIDGET_H
#define IP_WIDGET_H

#include "create_widgets/common/_checked_widget.h"
#include <QTabWidget>
#include "ip_widgets/_ip_widget.h"
#include "ip_widgets/addtab.h"

class IP_Widget : public _Checked_Widget
{
    Q_OBJECT
public:
    IP_Widget(
            QWidget *parent = Q_NULLPTR,
            QString  tag = "IP");

private:
    /* A network can have more than one of each family of address defined,
     * but only a single IPv4 address can have a dhcp or tftp element.
     * Similar to IPv4, one IPv6 address per network can also have
     * a dhcp definition.
     */
    AddTab          *addIP;
    QTabWidget      *ipSet;
    QWidget         *ipWdg;
    bool             IPv4HasDHCP = false;
    bool             IPv6HasDHCP = false;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);

private slots:
    void             updateDHCPUsage(uint, uint, bool);
    void             showCustomMenu(QPoint);
    void             addTab();
    void             closeTab(int);
};

#endif // IP_WIDGET_H
