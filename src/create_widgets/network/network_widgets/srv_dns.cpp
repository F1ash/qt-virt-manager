#include "srv_dns.h"

SRV_DNS::SRV_DNS(QWidget *parent, QString tag) :
    _List_Widget(parent, tag)
{
    serviceL = new QLabel("Service", this);
    domainL = new QLabel("Domain", this);
    targetL = new QLabel("Target", this);
    protocolL = new QLabel("Protocol", this);
    portL = new QLabel("Port", this);
    priorityL = new QLabel("Priority", this);
    weightL = new QLabel("Weight", this);
    service = new QLineEdit(this);
    service->setPlaceholderText("DNS_Name");
    domain = new QLineEdit(this);
    domain->setPlaceholderText("dns-domain-name");
    target = new QLineEdit(this);
    target->setPlaceholderText(".");
    protocol = new QComboBox(this);
    protocol->addItems(QStringList()<<"tcp"<<"udp");
    port = new QSpinBox(this);
    port->setRange(0, 65536);
    priority = new QSpinBox(this);
    priority->setRange(0, 1000);
    weight = new QSpinBox(this);
    weight->setRange(0, 1000);
    setsLayout = new QGridLayout();
    setsLayout->addWidget(serviceL, 0, 0);
    setsLayout->addWidget(service, 0, 1);
    setsLayout->addWidget(protocolL, 1, 0);
    setsLayout->addWidget(protocol, 1, 1);
    setsLayout->addWidget(domainL, 2, 0);
    setsLayout->addWidget(domain, 2, 1);
    setsLayout->addWidget(targetL, 3, 0);
    setsLayout->addWidget(target, 3, 1);
    setsLayout->addWidget(portL, 4, 0);
    setsLayout->addWidget(port, 4, 1);
    setsLayout->addWidget(priorityL, 5, 0);
    setsLayout->addWidget(priority, 5, 1);
    setsLayout->addWidget(weightL, 6, 0);
    setsLayout->addWidget(weight, 6, 1);
    sets = new QWidget(this);
    sets->setLayout(setsLayout);
    baseLayout->insertWidget(1, sets);
}

/* public slots */
QDomDocument SRV_DNS::getDataDocument() const
{
    QDomDocument doc;
    for (uint i=0; i<list->count(); i++) {
        QDomElement _srv = doc.createElement("srv");
        QString _data = list->item(i)->text();
        if ( _data.split(":").count()<7 ) continue;
        _srv.setAttribute(
                    "service", _data.split(":")[0]);
        _srv.setAttribute(
                    "protocol", _data.split(":")[1]);
        _srv.setAttribute(
                    "domain", _data.split(":")[2]);
        _srv.setAttribute(
                    "target", _data.split(":")[3]);
        _srv.setAttribute(
                    "port", _data.split(":")[4]);
        _srv.setAttribute(
                    "priority", _data.split(":")[5]);
        _srv.setAttribute(
                    "weight", _data.split(":")[6]);
        doc.appendChild(_srv);
    };
    return doc;
}
void SRV_DNS::addItem()
{
    QString _text = service->text();
    _text.append(":");
    _text.append(protocol->currentText());
    _text.append(":");
    _text.append(domain->text());
    _text.append(":");
    _text.append(target->text());
    _text.append(":");
    _text.append(port->text());
    _text.append(":");
    _text.append(priority->text());
    _text.append(":");
    _text.append(weight->text());
    if ( !_text.isEmpty() ) {
        bool exist = false;
        for (uint i=0; i<list->count(); i++) {
            if ( _text==list->item(i)->text() ) {
                exist = true;
                break;
            }
        };
        if ( !exist ) list->addItem(_text);
        service->clear();
        domain->clear();
        target->clear();
    };
}
