#ifndef MAC_WIDGET_H
#define MAC_WIDGET_H

#include "create_widgets/network/network_widgets/_checked_widget.h"

class MAC_Widget : public _Checked_Widget
{
    Q_OBJECT
public:
    MAC_Widget(
            QWidget *parent = NULL,
            QString  tag = "MAC");

private:
    QLineEdit       *mac;

public slots:
    QDomDocument     getDataDocument() const;
};

#endif // MAC_WIDGET_H
