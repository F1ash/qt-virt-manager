#include "dir_pool_stuff.h"

/*
 * http://libvirt.org/storage.html#StorageBackendDir
 */

Dir_Pool_Stuff::Dir_Pool_Stuff(
        QWidget *parent, virConnectPtr *connPtrPtr) :
    _Pool_Stuff(parent, connPtrPtr)
{
    source->setVisible(false);
    target->path->setPlaceholderText("/var/lib/virt/images");
}

void Dir_Pool_Stuff::setDataDescription(const QString &_xmlDesc)
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
                if ( _el.tagName()=="target" ) {
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
                            } else if ( _el1.tagName()=="encryption" ) {
                                target->encrypt->setUsage(true);
                                target->encrypt->setFormat(
                                            _el1.attribute("format", "default"));
                                QDomElement _secret = _el1.firstChildElement("secret");
                                if ( !_secret.isNull() ) {
                                    target->encrypt->setAutoSecretUsage(true);
                                    target->encrypt->setSecretUUID(
                                                _secret.attribute("uuid"));
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
QDomDocument Dir_Pool_Stuff::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _stuff, _target, _path, _perm,
            _encrypt, _secret;
    QDomText _text;
    _stuff = doc.createElement("stuff");
    doc.appendChild(_stuff);
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
    if ( target->encrypt->isUsed() ) {
        _encrypt = doc.createElement("encryption");
        _encrypt.setAttribute(
                    "format",
                    target->encrypt->getFormat());
        if ( !target->encrypt->AutoSecretIsUsed() ) {
            _secret = doc.createElement("secret");
            _secret.setAttribute("type", "passphrase");
            _secret.setAttribute("uuid", target->encrypt->getSecretUUID());
            _encrypt.appendChild(_secret);
        };
        _target.appendChild(_encrypt);
    };
    return doc;
}
