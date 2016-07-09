#ifndef ADDRESSING_WIDGET_H
#define ADDRESSING_WIDGET_H

#include "_checked_widget.h"
#include "addressing_widgets/dns_widget.h"
#include "addressing_widgets/ip_widget.h"
#include "addressing_widgets/mac_widget.h"
#include <QLineEdit>
#include <QSpinBox>
#include <QScrollArea>

class Addressing_Widget : public _Checked_Widget
{
    Q_OBJECT
public:
    explicit Addressing_Widget(
            QWidget *parent = nullptr,
            QString  tag = "Addressing");

private:
    DNS_Widget      *dns;
    IP_Widget       *ip;
    MAC_Widget      *mac;
    QScrollArea     *scroll;
    QWidget         *scrolled;
    QVBoxLayout     *scrollLayout;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
    void             ipv6Changed(bool);
};

#endif // ADDRESSING_WIDGET_H
