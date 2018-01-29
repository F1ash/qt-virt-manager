#include "dhcp_host.h"

DHCP_Host::DHCP_Host(
        QWidget *parent, QString tag) :
    _List_Widget(parent, tag)
{
    name = new QLineEdit(this);
    name->setPlaceholderText(tr("Host Name"));
    ip = new QLineEdit(this);
    ip->setPlaceholderText("IP");
    mac = new QLineEdit(this);
    mac->setPlaceholderText("MAC");
    baseLayout->insertWidget(1, name);
    panelLayout->insertWidget(1, ip);
    panelLayout->insertWidget(2, mac);
}

/* public slots */
QDomDocument DHCP_Host::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _host;
    for (int i=0; i<list->count(); i++) {
        QStringList _item = list->item(i)->text().split("|");
        _host = doc.createElement("host");
        _host.setAttribute("name", _item.first());
        _host.setAttribute("ip",   _item.at(1));
        if ( mac->isEnabled() )
            _host.setAttribute("mac",  _item.last());
        doc.appendChild(_host);
    };
    return doc;
}
void DHCP_Host::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _host = doc.documentElement();
    if ( !_host.isNull() ) {
        setUsage(true);
        name->setText(_host.attribute("name"));
        ip->setText(_host.attribute("ip"));
        mac->setText(_host.attribute("mac"));
        addItem();
    };
}
void DHCP_Host::addItem()
{
    if ( name->text().isEmpty() ) return;
    if ( ip->text().isEmpty() ) return;
    if ( mac->isEnabled() && name->text().isEmpty() ) return;
    QString _text;
    _text.append(name->text());
    _text.append("|");
    _text.append(ip->text());
    _text.append("|");
    _text.append(mac->text());
    bool exist = false;
    for (int i=0; i<list->count(); i++) {
        if ( list->item(i)->text()==_text ) {
            exist = true;
            break;
        };
    };
    if ( !exist ) {
        list->addItem(_text);
        name->clear();
        ip->clear();
        mac->clear();
    };
}
void DHCP_Host::setFreezMAC(bool state)
{
    mac->clear();
    mac->setDisabled(state);
}
