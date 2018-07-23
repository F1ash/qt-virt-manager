#ifndef DHCP_WIDGET_H
#define DHCP_WIDGET_H

#include "dhcp_widgets/dhcp_host.h"
#include "dhcp_widgets/dhcp_range.h"

class DHCP_Widget : public _Checked_Widget
{
    Q_OBJECT
public:
    explicit DHCP_Widget(
            QWidget *parent = Q_NULLPTR,
            QString  tag = "");

private:
    DHCP_Host       *host;
    DHCP_Range      *range;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
};

#endif // DHCP_WIDGET_H
