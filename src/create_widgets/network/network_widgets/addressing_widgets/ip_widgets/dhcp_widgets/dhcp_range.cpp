#include "dhcp_range.h"

DHCP_Range::DHCP_Range(QWidget *parent, QString tag) :
    _List_Widget(parent, tag)
{
    start = new QLineEdit(this);
    start->setPlaceholderText("start IP");
    end = new QLineEdit(this);
    end->setPlaceholderText("end IP");
    baseLayout->insertWidget(1, start);
    baseLayout->insertWidget(2, end);
}

/* public slots */
QDomDocument DHCP_Range::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _range;
    for (int i=0; i<list->count(); i++) {
        QListWidgetItem *item = list->item(i);
        if ( nullptr==item ) continue;
        QStringList _item = item->text().split("|");
        _range = doc.createElement("range");
        _range.setAttribute("start", _item.first());
        _range.setAttribute("end",   _item.last());
        doc.appendChild(_range);
    };
    return doc;
}
void DHCP_Range::setDataDescription(QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _range = doc.documentElement();
    if ( !_range.isNull() ) {
        setUsage(true);
        start->setText(_range.attribute("start"));
        end->setText(_range.attribute("end"));
        addItem();
    };
}
void DHCP_Range::addItem()
{
    if ( start->text().isEmpty() ) return;
    if ( end->text().isEmpty() ) return;
    QString _text;
    _text.append(start->text());
    _text.append("|");
    _text.append(end->text());
    bool exist = false;
    for (int i=0; i<list->count(); i++) {
        if ( list->item(i)->text()==_text ) {
            exist = true;
            break;
        };
    };
    if ( !exist ) {
        list->addItem(_text);
        start->clear();
        end->clear();
    };
}
