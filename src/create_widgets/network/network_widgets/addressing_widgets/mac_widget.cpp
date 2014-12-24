#include "mac_widget.h"

MAC_Widget::MAC_Widget(QWidget *parent, QString tag) :
    _Checked_Widget(parent, tag)
{
    mac = new QLineEdit(this);
    mac->setPlaceholderText("52:54:00:1C:DA:2F");
    mac->setMinimumWidth(150);
    baseLayout->addWidget(mac, 0, Qt::AlignLeft);
}

/* public slots */
QDomDocument MAC_Widget::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _mac;
    _mac = doc.createElement("mac");
    _mac.setAttribute("address", mac->text());
    doc.appendChild(_mac);
    return doc;
}
