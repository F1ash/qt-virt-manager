#include "netfs_pool_stuff.h"

/*
 * http://libvirt.org/storage.html#StorageBackendNetFS
 */

#define NETFS_POOL_FORMAT_TYPES QStringList()\
    <<"auto"\
    <<"nfs"\
    <<"glusterfs"\
    <<"cifs"

NetFs_Pool_Stuff::NetFs_Pool_Stuff(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    _Pool_Stuff(parent, connPtrPtr)
{
    source->hostLabel->setVisible(true);
    source->host->setVisible(true);
    source->host->setUsage(true);
    source->host->setHostPlaceholderText("nfs.example.com");
    source->host->setFullHostMode(false);
    source->dirLabel->setVisible(true);
    source->formatLabel->setVisible(true);
    source->dir->setVisible(true);
    source->format->setVisible(true);
    source->format->addItems(NETFS_POOL_FORMAT_TYPES);
    source->dir->setPlaceholderText("/var/lib/virt/images");
    target->path->setPlaceholderText("/var/lib/virt/images");
}

void NetFs_Pool_Stuff::setDataDescription(const QString &_xmlDesc)
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
                if ( _el.tagName().compare("source")==0 ) {
                    QDomNode _n1 = _el.firstChild();
                    while ( !_n1.isNull() ) {
                        QDomElement _el1 = _n1.toElement();
                        if ( !_el1.isNull() ) {
                            if ( _el1.tagName().compare("host")==0 ) {
                                source->host->setHostItem(_el1.attribute("name"));
                            } else if ( _el1.tagName().compare("dir")==0 ) {
                                source->dir->setText(_el1.attribute("path", ""));
                            } else if ( _el1.tagName().compare("format")==0 ) {
                                QString _fmt = _el1.attribute("type", "auto");
                                int idx = source->format->findText(_fmt);
                                if ( idx<0 ) idx = 0;
                                source->format->setCurrentIndex(idx);
                            };
                        };
                        _n1 = _n1.nextSibling();
                    };
                } else if ( _el.tagName().compare("target")==0 ) {
                    QDomNode _n1 = _el.firstChild();
                    while ( !_n1.isNull() ) {
                        QDomElement _el1 = _n1.toElement();
                        if ( !_el1.isNull() ) {
                            if ( _el1.tagName().compare("path")==0 ) {
                                target->path->setText(_el1.text());
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
QDomDocument NetFs_Pool_Stuff::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _stuff, _source, _host, _dir, _format,
            _target, _perm, _path;
    QDomText _text;
    _stuff = doc.createElement("stuff");
    doc.appendChild(_stuff);

    _source = doc.createElement("source");
    _stuff.appendChild(_source);
    foreach (QString _data, source->host->getHostsList()) {
        if ( _data.isEmpty() ) continue;
        QString _name, _port;
        QStringList _split = _data.split(":");
        if ( _split.count()>1 ) _port = _split.last();
        _name = _split.first();
        _host = doc.createElement("host");
        _host.setAttribute("name", _name);
        if ( !_port.isEmpty() ) _host.setAttribute("port", _port);
        _source.appendChild(_host);
    };
    _dir = doc.createElement("dir");
    _dir.setAttribute("path", source->dir->text());
    _source.appendChild(_dir);
    _format = doc.createElement("format");
    _format.setAttribute(
                "type",
                source->format->currentText().toLower());
    _source.appendChild(_format);

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
