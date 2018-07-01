#include "fs_pool_stuff.h"

/*
 * http://libvirt.org/storage.html#StorageBackendFS
 */

#define FS_POOL_FORMAT_TYPES QStringList()\
    <<"auto"\
    <<"ext2"\
    <<"ext3"\
    <<"ext4"\
    <<"ufs"\
    <<"iso9660"\
    <<"udf"\
    <<"gfs"\
    <<"gfs2"\
    <<"vfat"\
    <<"hfs+"\
    <<"xfs"\
    <<"ocfs2"

Fs_Pool_Stuff::Fs_Pool_Stuff(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    _Pool_Stuff(parent, connPtrPtr)
{
    source->deviceLabel->setVisible(true);
    //source->formatLabel->setVisible(true);
    source->device->setVisible(true);
    source->device->name->setPlaceholderText("/dev/VolGroup00/VirtImages");
    //source->format->setVisible(true);
    source->format->addItems(FS_POOL_FORMAT_TYPES);
    target->path->setPlaceholderText("/var/lib/virt/images");
}

void Fs_Pool_Stuff::setDataDescription(const QString &_xmlDesc)
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
                            if ( _el1.tagName().compare("device")==0 ) {
                                source->device->addNewDevicePath(
                                            _el1.attribute("path"));
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
                            } else if ( _el1.tagName().compare("permissions")==0 ) {
                                target->usePerm->setChecked(true);
                                QDomNode _n2 = _el1.firstChild();
                                while ( !_n2.isNull() ) {
                                    QDomElement _el2 = _n2.toElement();
                                    if ( !_el2.isNull() ) {
                                        if ( _el2.tagName().compare("owner")==0 ) {
                                            target->owner->setText(_el2.text());
                                        } else if ( _el2.tagName().compare("group")==0 ) {
                                            target->group->setText(_el2.text());
                                        } else if ( _el2.tagName().compare("mode")==0 ) {
                                            target->mode->setText(_el2.text());
                                        } else if ( _el2.tagName().compare("label")==0 ) {
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
QDomDocument Fs_Pool_Stuff::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _stuff, _source, _device, _format,
            _target, _perm, _path;
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
    //_format = doc.createElement("format");
    //_format.setAttribute(
    //            "type",
    //            source->format->currentText().toLower());
    //_source.appendChild(_format);

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
