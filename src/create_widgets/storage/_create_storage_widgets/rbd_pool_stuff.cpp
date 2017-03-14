#include "rbd_pool_stuff.h"

/*
 * http://libvirt.org/storage.html#StorageBackendRBD
 */

RBD_Pool_Stuff::RBD_Pool_Stuff(
        QWidget *parent, virConnectPtr *connPtrPtr) :
    _Pool_Stuff(parent, connPtrPtr)
{
    source->namedLabel->setVisible(true);
    source->named->setVisible(true);
    source->hostLabel->setVisible(true);
    source->host->setVisible(true);
    source->host->setUsage(true);
    source->host->setHostPlaceholderText("my.ceph.monitor");
    source->host->setPortPlaceholderText("6789");
    source->authLabel->setVisible(true);
    source->auth->setVisible(true);
    source->auth->setSecretType("CEPH");
    target->setVisible(false);
}

void RBD_Pool_Stuff::setDataDescription(const QString &_xmlDesc)
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
                                source->host->setHostItem(_el1.attribute("name"));
                            } else if ( _el1.tagName()=="auth" ) {
                                source->auth->auth->setChecked(true);
                                source->auth->userName->setText(
                                            _el1.attribute("username"));
                                source->auth->setSecretType("CEPH");
                                QDomElement _secret = _el1.firstChildElement("secret");
                                if ( !_secret.isNull() ) {
                                    if ( _secret.hasAttribute("uuid") ) {
                                        source->auth->usageType->setCurrentIndex(1); // UUID type
                                        source->auth->usage->setText(
                                                    _secret.attribute("uuid"));
                                    } else if ( _secret.hasAttribute("usage") ) {
                                        source->auth->usageType->setCurrentIndex(0); // Usage type
                                        source->auth->usage->setText(
                                                    _secret.attribute("usage"));
                                    };
                                };
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
QDomDocument RBD_Pool_Stuff::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _stuff, _source, _name, _host, _auth, _secret;
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
    if ( source->auth->auth->isChecked() ) {
        _auth = doc.createElement("auth");
        _auth.setAttribute("type", "ceph");
        _auth.setAttribute(
                    "username",
                    source->auth->userName->text());
        _secret = doc.createElement("secret");
        _secret.setAttribute(
                    source->auth->usageType->currentText().toLower(),
                    source->auth->usage->text());
        _auth.appendChild(_secret);
        _source.appendChild(_auth);
    };
    return doc;
}
