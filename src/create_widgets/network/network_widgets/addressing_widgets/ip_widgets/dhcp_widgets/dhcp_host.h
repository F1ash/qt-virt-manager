#ifndef DHCP_HOST_H
#define DHCP_HOST_H

#include "create_widgets/common/_list_widget.h"

class DHCP_Host : public _List_Widget
{
    Q_OBJECT
public:
    explicit DHCP_Host(
            QWidget *parent = Q_NULLPTR,
            QString  tag = "");

private:
    QLineEdit       *name, *ip, *mac;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
    void             addItem();
    void             setFreezMAC(bool);
};

#endif // DHCP_HOST_H
