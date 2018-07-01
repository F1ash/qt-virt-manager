#include "txt_dns.h"

TXT_DNS::TXT_DNS(
        QWidget *parent, QString tag) :
    _List_Widget(parent, tag)
{
    comma = new QLabel(":", this);
    hostValue = new QLineEdit(this);
    hostValue->setPlaceholderText("value1, value2,...");
    hostName = new QLineEdit(this);
    hostName->setPlaceholderText("DNS_Host_Name");
    panelLayout->insertWidget(1, hostName);
    panelLayout->insertWidget(2, comma);
    panelLayout->insertWidget(3, hostValue);
}

/* public slots */
QDomDocument TXT_DNS::getDataDocument() const
{
    QDomDocument doc;
    for (int i=0; i<list->count(); i++) {
        QDomElement _txt = doc.createElement("txt");
        QString _data = list->item(i)->text();
        _txt.setAttribute(
                    "name", _data.split(":").first());
        _txt.setAttribute(
                    "value", _data.split(":").last());
        doc.appendChild(_txt);
    };
    return doc;
}
void TXT_DNS::setDataDescription(const QString &_xmlDesc)
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
            if ( _el.tagName().compare("txt")==0 ) {
                hostName->setText(
                            _el.attribute("name"));
                hostValue->setText(
                            _el.attribute("value"));
                addItem();
            };
        };
        _n = _n.nextSibling();
    };
}
void TXT_DNS::addItem()
{
    QString _text = hostName->text();
    _text.append(":");
    _text.append(hostValue->text());
    if ( !_text.isEmpty() ) {
        bool exist = false;
        for (int i=0; i<list->count(); i++) {
            if ( _text.compare(list->item(i)->text())==0 ) {
                exist = true;
                break;
            }
        };
        if ( !exist ) list->addItem(_text);
        hostName->clear();
        hostValue->clear();
    };
}
