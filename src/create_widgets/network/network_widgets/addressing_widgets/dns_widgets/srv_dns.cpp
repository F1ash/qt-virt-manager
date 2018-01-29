#include "srv_dns.h"

SRV_DNS::SRV_DNS(
        QWidget *parent, QString tag) :
    _List_Widget(parent, tag)
{
    serviceL = new QLabel("Service", this);
    domainL = new QLabel("Domain", this);
    targetL = new QLabel("Target", this);
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
    port->setRange(0, 65535);
    priority = new QSpinBox(this);
    priority->setRange(0, 1000);
    weight = new QSpinBox(this);
    weight->setRange(0, 1000);
    mandatoryLayout = new QHBoxLayout();
    mandatoryLayout->addWidget(serviceL);
    mandatoryLayout->addWidget(service);
    mandatoryLayout->addWidget(protocol);
    mandatory = new QWidget(this);
    mandatory->setLayout(mandatoryLayout);
    lettersLayout = new QGridLayout();
    lettersLayout->addWidget(domainL, 0, 0);
    lettersLayout->addWidget(targetL, 0, 1);
    lettersLayout->addWidget(domain, 1, 0);
    lettersLayout->addWidget(target, 1, 1);
    numbersLayout = new QGridLayout();
    numbersLayout->addWidget(portL, 0, 0);
    numbersLayout->addWidget(priorityL, 0, 1);
    numbersLayout->addWidget(weightL, 0, 2);
    numbersLayout->addWidget(port, 1, 0);
    numbersLayout->addWidget(priority, 1, 1);
    numbersLayout->addWidget(weight, 1, 2);
    letters = new QWidget(this);
    letters->setLayout(lettersLayout);
    numbers = new QWidget(this);
    numbers->setLayout(numbersLayout);
    baseLayout->insertWidget(1, mandatory);
    baseLayout->insertWidget(2, letters);
    panelLayout->insertWidget(1, numbers);
}

/* public slots */
QDomDocument SRV_DNS::getDataDocument() const
{
    QDomDocument doc;
    for (int i=0; i<list->count(); i++) {
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
void SRV_DNS::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _network, _dns, _el;
    _network = doc.documentElement();
    _dns = _network.firstChildElement("dns");
    QDomNode _n = _dns.firstChild();
    while ( !_n.isNull() ) {
        setUsage(true);
        _el = _n.toElement();
        if ( !_el.isNull() ) {
            if ( _el.tagName()=="srv" ) {
                service->setText(
                            _el.attribute("service"));
                domain->setText(
                            _el.attribute("domain"));
                target->setText(
                            _el.attribute("target"));
                QString _prot, _port, _prior, _w;
                _prot = _el.attribute("protocol");
                _port = _el.attribute("port");
                _prior = _el.attribute("priority");
                _w = _el.attribute("weight");
                int idx = protocol->findText(_prot);
                if ( idx<0 ) idx = 0;
                protocol->setCurrentIndex(idx);
                port->setValue(_port.toInt());
                priority->setValue(_prior.toInt());
                weight->setValue(_w.toInt());
                addItem();
            };
        };
        _n = _n.nextSibling();
    };
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
        for (int i=0; i<list->count(); i++) {
            if ( _text==list->item(i)->text() ) {
                exist = true;
                break;
            }
        };
        if ( !exist ) list->addItem(_text);
        service->clear();
        domain->clear();
        target->clear();
        port->clear();
        priority->clear();
        weight->clear();
    };
}
