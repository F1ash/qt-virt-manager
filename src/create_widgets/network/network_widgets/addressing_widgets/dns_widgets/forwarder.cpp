#include "forwarder.h"

Forwarder::Forwarder(QWidget *parent, QString tag) :
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
    QDomElement _network, _fwd;
    _network = doc.firstChildElement("network");
    if ( !_network.isNull() ) {
        _fwd = _network.firstChildElement("forwarder");
        if ( !_fwd.isNull() ) {
            setUsage(true);
        };
    };
}
void Forwarder::addItem()
{
    QString _text = frwds->text();
    if ( !_text.isEmpty() ) {
        bool exist = false;
        for (int i=0; i<list->count(); i++) {
            if ( _text==list->item(i)->text() ) {
                exist = true;
                break;
            }
        };
        if ( !exist ) list->addItem(_text);
        frwds->clear();
    };
}
