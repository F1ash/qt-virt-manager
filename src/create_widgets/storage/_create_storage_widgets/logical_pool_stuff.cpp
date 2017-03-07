#include "logical_pool_stuff.h"

/*
 * http://libvirt.org/storage.html#StorageBackendLogical
 */

#define LOGICAL_POOL_FORMAT_TYPES QStringList()\
    <<"auto"\
    <<"lvm2"

Logical_Pool_Stuff::Logical_Pool_Stuff(
        QWidget *parent, virConnectPtr *connPtrPtr) :
    _Pool_Stuff(parent, connPtrPtr)
{
    source->deviceLabel->setVisible(true);
    source->formatLabel->setVisible(true);
    source->device->setVisible(true);
    source->device->setOneDeviceMode(true);
    source->format->setVisible(true);
    source->format->addItems(LOGICAL_POOL_FORMAT_TYPES);
    source->namedLabel->setVisible(true);
    source->named->setVisible(true);
    target->path->setPlaceholderText("/dev/<LVM volume group name>");
}

void Logical_Pool_Stuff::setDataDescription(const QString &_xmlDesc)
{

}
QDomDocument Logical_Pool_Stuff::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _stuff, _source, _device, _format,
            _name, _target, _perm, _encrypt, _path;
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
    if ( !source->named->text().isEmpty() ) {
        _name = doc.createElement("name");
        _source.appendChild(_name);
        _text = doc.createTextNode(source->named->text());
        _name.appendChild(_text);
    };
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
    if ( target->encrypt->isUsed() ) {
        _encrypt = doc.createElement("encryption");
        _target.appendChild(_encrypt);
        _encrypt.setAttribute(
                    "format",
                    target->encrypt->getFormat());
    };
    return doc;
}
