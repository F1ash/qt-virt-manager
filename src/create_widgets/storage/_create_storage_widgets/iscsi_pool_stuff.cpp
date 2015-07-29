#include "iscsi_pool_stuff.h"

/*
 * http://libvirt.org/storage.html#StorageBackendISCSI
 */

iSCSI_Pool_Stuff::iSCSI_Pool_Stuff(QWidget *parent, virConnectPtr _conn) :
    _Pool_Stuff(parent, _conn)
{
    source->deviceLabel->setVisible(true);
    source->device->setVisible(true);
    source->device->setOneDeviceMode(true);
    source->device->name->setPlaceholderText("iqn.2013-06.com.example:iscsi-pool");
    source->hostLabel->setVisible(true);
    source->host->setVisible(true);
    source->host->checkHosts(true);
    source->host->setFullHostMode(false);
    source->host->setOneHostMode(true);
    source->host->setHostPlaceholderText("iscsi.example.com");
    source->authLabel->setVisible(true);
    source->auth->setVisible(true);
    source->auth->setSecretType("ISCSI");
    target->path->setPlaceholderText("/dev/disk/{by-path, by-id}");
}

/* public slots */
QDomDocument iSCSI_Pool_Stuff::getStorageXMLDesc() const
{
    QDomDocument doc;
    QDomElement _stuff, _source, _device, _host, _auth, _secret,
            _target, _path, _perm, _encrypt;
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
    if ( target->encrypt->isUsed() ) {
        _encrypt = doc.createElement("encrypt");
        _target.appendChild(_encrypt);
        _encrypt.setAttribute(
                    "format",
                    target->encrypt->getFormat());
    };
    return doc;
}
