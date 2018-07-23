#ifndef FORWARDER_H
#define FORWARDER_H

#include "create_widgets/common/_list_widget.h"

class Forwarder : public _List_Widget
{
    Q_OBJECT
public:
    Forwarder(
            QWidget *parent = Q_NULLPTR,
            QString  tag    = "Forwarders");

private:
    QLineEdit       *frwds;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
    void             addItem();
};

#endif // FORWARDER_H
