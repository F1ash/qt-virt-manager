#ifndef ADDRESSING_WIDGET_H
#define ADDRESSING_WIDGET_H

#include "_checked_widget.h"
#include "dns_widget.h"
#include "ip_widget.h"
#include "mac_widget.h"
#include <QLineEdit>
#include <QSpinBox>

class Addressing_Widget : public _Checked_Widget
{
    Q_OBJECT
public:
    explicit Addressing_Widget(QWidget *parent = NULL,
            QString tag = "Addressing");

private:
    DNS_Widget      *dns;
    IP_Widget       *ip;
    MAC_Widget      *mac;

public slots:
    QDomDocument     getDataDocument() const;
};

#endif // ADDRESSING_WIDGET_H
