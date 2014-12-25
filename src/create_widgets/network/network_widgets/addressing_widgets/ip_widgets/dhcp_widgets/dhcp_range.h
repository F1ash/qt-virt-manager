#ifndef DHCP_RANGE_H
#define DHCP_RANGE_H

#include "create_widgets/network/network_widgets/_list_widget.h"

class DHCP_Range : public _List_Widget
{
    Q_OBJECT
public:
    explicit DHCP_Range(
            QWidget *parent = NULL,
            QString  tag = "");

private:
    QLineEdit       *start, *end;

public slots:
    QDomDocument     getDataDocument() const;
    void             addItem();
};

#endif // DHCP_RANGE_H
