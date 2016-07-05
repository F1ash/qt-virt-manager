#include "dhcp_widget.h"

DHCP_Widget::DHCP_Widget(QWidget *parent, QString tag) :
    _Checked_Widget(parent, tag)
{
    host = new DHCP_Host(this, "HOSTs");
    range = new DHCP_Range(this, "RANGEs");
    baseLayout->insertWidget(1, host);
    baseLayout->insertWidget(2, range);
}

/* public slots */
QDomDocument DHCP_Widget::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _dhcp = doc.createElement("dhcp");
    if ( host->isUsed() ) {
        _dhcp.appendChild(
                    host->getDataDocument());
    };
    if ( range->isUsed() ) {
        _dhcp.appendChild(
                    range->getDataDocument());
    };
    doc.appendChild(_dhcp);
    return doc;
}
void DHCP_Widget::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _dhcp = doc.documentElement();
    if ( !_dhcp.isNull() ) {
        setUsage(true);
        QDomNode n = _dhcp.firstChild();
        while ( !n.isNull() ) {
            QDomElement e = n.toElement();
            if ( !e.isNull() ) {
                QDomDocument _doc;
                _doc.setContent(QString());
                _doc.appendChild(e.cloneNode());
                QString _xml = _doc.toString();
                if ( e.tagName()=="host" ) {
                    host->setDataDescription(_xml);
                } else if ( e.tagName()=="range" ) {
                    range->setDataDescription(_xml);
                };
            };
            n = n.nextSibling();
        };
    };
}
