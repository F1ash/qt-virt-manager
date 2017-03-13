#include "sheepdog_pool_stuff.h"

/*
 * http://libvirt.org/storage.html#StorageBackendSheepdog
 */

SheepDog_Pool_Stuff::SheepDog_Pool_Stuff(
        QWidget *parent, virConnectPtr *connPtrPtr) :
    _Pool_Stuff(parent, connPtrPtr)
{
    source->namedLabel->setVisible(true);
    source->named->setVisible(true);
    source->hostLabel->setVisible(true);
    source->host->setVisible(true);
    source->host->setUsage(true);
    source->host->setOneHostMode(true);
    source->host->setHostPlaceholderText("localhost");
    source->host->setPortPlaceholderText("7000");
    target->setVisible(false);
}

void SheepDog_Pool_Stuff::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _pool;
    _pool = doc.firstChildElement("pool");
    if ( !_pool.isNull() ) {
        QDomNode _n = _pool.firstChild();
        while ( !_n.isNull() ) {
            QDomElement _el = _n.toElement();
            if ( !_el.isNull() ) {
                if ( _el.tagName()=="source" ) {
                    QDomNode _n1 = _el.firstChild();
                    while ( !_n1.isNull() ) {
                        QDomElement _el1 = _n1.toElement();
                        if ( !_el1.isNull() ) {
                            if ( _el1.tagName()=="name" ) {
                                source->named->setText(_el1.text());
                            } else if ( _el1.tagName()=="host" ) {
                                QString _host = QString("%1:%2")
                                        .arg(_el1.attribute("name"))
                                        .arg(_el1.attribute("port", "7000"));
                                source->host->setHostItem(_host);
                            };
                        };
                        _n1 = _n1.nextSibling();
                    };
                };
            };
            _n = _n.nextSibling();
        };
    };
}
QDomDocument SheepDog_Pool_Stuff::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _stuff, _source, _name, _host;
    QDomText _text;
    _stuff = doc.createElement("stuff");
    doc.appendChild(_stuff);

    _source = doc.createElement("source");
    _stuff.appendChild(_source);
    if ( !source->named->text().isEmpty() ) {
        _name = doc.createElement("name");
        _source.appendChild(_name);
        _text = doc.createTextNode(source->named->text());
        _name.appendChild(_text);
    };
    foreach (QString _addr, source->host->getHostsList()) {
        if ( _addr.isEmpty() ) continue;
        QString host, port;
        QStringList _split = _addr.split(":");
        host.append(_split.first());
        if ( _split.count()>1 ) port.append(_split.last());
        _host = doc.createElement("host");
        _host.setAttribute("name", host);
        _host.setAttribute("port", port);
        _source.appendChild(_host);
    };
    return doc;
}
