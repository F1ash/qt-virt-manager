#include "rbd_pool_stuff.h"

/*
 * http://libvirt.org/storage.html#StorageBackendRBD
 */

RBD_Pool_Stuff::RBD_Pool_Stuff(QWidget *parent) :
    _Pool_Stuff(parent)
{
    source->namedLabel->setVisible(true);
    source->named->setVisible(true);
    source->hostLabel->setVisible(true);
    source->host->setVisible(true);
    source->host->checkHosts(true);
    source->authLabel->setVisible(true);
    source->auth->setVisible(true);
    target->setVisible(false);
}

/* public slots */
QDomDocument RBD_Pool_Stuff::getStorageXMLDesc() const
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
