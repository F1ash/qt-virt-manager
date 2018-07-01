#include "forwarder.h"

Forwarder::Forwarder(
        QWidget *parent, QString tag) :
    _List_Widget(parent, tag)
{
    frwds = new QLineEdit(this);
    frwds->setPlaceholderText("8.8.8.8");
    panelLayout->insertWidget(1, frwds);
}

/* public slots */
QDomDocument Forwarder::getDataDocument() const
{
    QDomDocument doc;
    for (int i=0; i<list->count(); i++) {
        QDomElement _el;
        _el = doc.createElement("forwarder");
        _el.setAttribute(
                    "addr",
                    list->item(i)->text());
        doc.appendChild(_el);
    };
    return doc;
}
void Forwarder::setDataDescription(const QString &_xmlDesc)
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
            if ( _el.tagName().compare("forwarder")==0 ) {
                frwds->setText(
                            _el.attribute("addr"));
                addItem();
            };
        };
        _n = _n.nextSibling();
    };
}
void Forwarder::addItem()
{
    QString _text = frwds->text();
    if ( !_text.isEmpty() ) {
        bool exist = false;
        for (int i=0; i<list->count(); i++) {
            if ( _text.compare(list->item(i)->text())==0 ) {
                exist = true;
                break;
            }
        };
        if ( !exist ) list->addItem(_text);
        frwds->clear();
    };
}
