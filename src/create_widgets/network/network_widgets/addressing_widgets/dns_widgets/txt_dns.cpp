#include "txt_dns.h"

TXT_DNS::TXT_DNS(QWidget *parent, QString tag) :
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
void TXT_DNS::setDataDescription(QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _network, _txt;
    _network = doc.firstChildElement("network");
    if ( !_network.isNull() ) {
        _txt = _network.firstChildElement("txt");
        if ( !_txt.isNull() ) {
            setUsage(true);
        };
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
            if ( _text==list->item(i)->text() ) {
                exist = true;
                break;
            }
        };
        if ( !exist ) list->addItem(_text);
        hostName->clear();
        hostValue->clear();
    };
}
