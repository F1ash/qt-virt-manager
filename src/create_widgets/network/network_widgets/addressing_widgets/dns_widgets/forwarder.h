#ifndef FORWARDER_H
#define FORWARDER_H

#include "create_widgets/network/network_widgets/_list_widget.h"

class Forwarder : public _List_Widget
{
    Q_OBJECT
public:
    Forwarder(
            QWidget *parent = NULL,
            QString  tag = "Forwarders");

private:
    QLineEdit       *frwds;

public slots:
    QDomDocument     getDataDocument() const;
    void             addItem();
};

#endif // FORWARDER_H
