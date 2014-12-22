#ifndef DNS_WIDGET_H
#define DNS_WIDGET_H

#include "forwarder.h"
#include "host_dns.h"
#include "txt_dns.h"

class DNS_Widget : public _Checked_Widget
{
    Q_OBJECT
public:
    DNS_Widget(
            QWidget *parent = NULL,
            QString  tag = "DNS");

private:
    QCheckBox       *forwardPlainNames;
    Forwarder       *forwarder;
    Host_DNS        *hosts;
    TXT_DNS         *txts;

public slots:
    QDomDocument     getDataDocument() const;
};

#endif // DNS_WIDGET_H
