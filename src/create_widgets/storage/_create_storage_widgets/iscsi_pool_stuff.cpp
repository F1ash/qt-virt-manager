#include "iscsi_pool_stuff.h"

/*
 * http://libvirt.org/storage.html#StorageBackendISCSI
 */

iSCSI_Pool_Stuff::iSCSI_Pool_Stuff(
        QWidget *parent, virConnectPtr *connPtrPtr) :
    _Pool_Stuff(parent, connPtrPtr)
{
    source->deviceLabel->setVisible(true);
    source->device->setVisible(true);
    source->device->setOneDeviceMode(true);
    source->device->name->setPlaceholderText("iqn.2013-06.com.example:iscsi-pool");
    source->hostLabel->setVisible(true);
    source->host->setVisible(true);
    source->host->setUsage(true);
    source->host->setFullHostMode(false);
    source->host->setOneHostMode(true);
    source->host->setHostPlaceholderText("iscsi.example.com");
    source->authLabel->setVisible(true);
    source->auth->setVisible(true);
    source->auth->setSecretType("ISCSI");
    target->path->setPlaceholderText("/dev/disk/{by-path, by-id}");
}

void iSCSI_Pool_Stuff::setDataDescription(const QString &_xmlDesc)
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
                            if ( _el1.tagName()=="host" ) {
                                source->host->setHostItem(_el1.attribute("name"));
                            } else if ( _el1.tagName()=="device" ) {
                                source->device->addNewDevicePath(
                                            _el1.attribute("path"));
                            } else if ( _el1.tagName()=="auth" ) {
                                source->auth->auth->setChecked(true);
                                source->auth->userName->setText(
                                            _el1.attribute("username"));
                                source->auth->setSecretType("CHAP");
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
                } else if ( _el.tagName()=="target" ) {
                    QDomNode _n1 = _el.firstChild();
                    while ( !_n1.isNull() ) {
                        QDomElement _el1 = _n1.toElement();
                        if ( !_el1.isNull() ) {
                            if ( _el1.tagName()=="path" ) {
                                target->path->setText(_el1.text());
                            } else if ( _el1.tagName()=="permissions" ) {
                                target->usePerm->setChecked(true);
                                QDomNode _n2 = _el1.firstChild();
                                while ( !_n2.isNull() ) {
                                    QDomElement _el2 = _n2.toElement();
                                    if ( !_el2.isNull() ) {
                                        if ( _el2.tagName()=="owner" ) {
                                            target->owner->setText(_el2.text());
                                        } else if ( _el2.tagName()=="group" ) {
                                            target->group->setText(_el2.text());
                                        } else if ( _el2.tagName()=="mode" ) {
                                            target->mode->setText(_el2.text());
                                        } else if ( _el2.tagName()=="label" ) {
                                            target->label->setText(_el2.text());
                                        };
                                    };
                                    _n2 = _n2.nextSibling();
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
QDomDocument iSCSI_Pool_Stuff::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _stuff, _source, _device, _host, _auth, _secret,
            _target, _path, _perm;
    QDomText _text;
    _stuff = doc.createElement("stuff");
    doc.appendChild(_stuff);

    _source = doc.createElement("source");
    _stuff.appendChild(_source);
    foreach (QString _dir, source->device->getDevicesList()) {
        if ( _dir.isEmpty() ) continue;
        _device = doc.createElement("device");
        _device.setAttribute("path", _dir);
        _source.appendChild(_device);
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
        _auth.setAttribute("type", "chap");
        _auth.setAttribute(
                    "username",
                    source->auth->userName->text());
        _secret = doc.createElement("secret");
        _secret.setAttribute("type", "iscsi");
        _secret.setAttribute(
                    source->auth->usageType->currentText().toLower(),
                    source->auth->usage->text());
        _auth.appendChild(_secret);
        _source.appendChild(_auth);
    };

    _target = doc.createElement("target");
    _stuff.appendChild(_target);
    _path = doc.createElement("path");
    _text = doc.createTextNode(target->path->text());
    _path.appendChild(_text);
    _target.appendChild(_path);
    if ( target->usePerm->isChecked() ) {
        QDomElement _owner, _group, _mode, _label;
        _perm = doc.createElement("permissions");
        _target.appendChild(_perm);
        _owner = doc.createElement("owner");
        _text = doc.createTextNode(target->owner->text());
        _owner.appendChild(_text);
        _perm.appendChild(_owner);
        _group = doc.createElement("group");
        _text = doc.createTextNode(target->group->text());
        _group.appendChild(_text);
        _perm.appendChild(_group);
        _mode = doc.createElement("mode");
        _text = doc.createTextNode(target->mode->text());
        _mode.appendChild(_text);
        _perm.appendChild(_mode);
        _label = doc.createElement("label");
        _text = doc.createTextNode(target->label->text());
        _label.appendChild(_text);
        _perm.appendChild(_label);
    };
    return doc;
}
