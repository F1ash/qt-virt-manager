#ifndef DHCP_RANGE_H
#define DHCP_RANGE_H

#include "create_widgets/common/_list_widget.h"

class DHCP_Range : public _List_Widget
{
    Q_OBJECT
public:
    explicit DHCP_Range(
            QWidget *parent = nullptr,
            QString  tag = "");

private:
    QLineEdit       *start, *end;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
    void             addItem();
};

#endif // DHCP_RANGE_H
