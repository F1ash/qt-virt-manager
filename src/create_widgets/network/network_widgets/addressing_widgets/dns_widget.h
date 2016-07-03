#ifndef DNS_WIDGET_H
#define DNS_WIDGET_H

#include "dns_widgets/forwarder.h"
#include "dns_widgets/host_dns.h"
#include "dns_widgets/txt_dns.h"
#include "dns_widgets/srv_dns.h"

class DNS_Widget : public _Checked_Widget
{
    Q_OBJECT
public:
    DNS_Widget(
            QWidget *parent = nullptr,
            QString  tag = "DNS");

private:
    QCheckBox       *forwardPlainNames;
    Forwarder       *forwarder;
    Host_DNS        *hosts;
    TXT_DNS         *txts;
    SRV_DNS         *srvs;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);
};

#endif // DNS_WIDGET_H
