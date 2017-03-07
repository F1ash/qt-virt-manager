#include "zfs_pool_stuff.h"

/*
 * http://libvirt.org/storage.html#StorageBackendZFS
 */

ZFS_Pool_Stuff::ZFS_Pool_Stuff(
        QWidget *parent, virConnectPtr *connPtrPtr) :
    _Pool_Stuff(parent, connPtrPtr)
{
    source->namedLabel->setVisible(true);
    source->named->setVisible(true);
    source->deviceLabel->setVisible(true);
    source->device->setVisible(true);
    source->host->setUsage(true);
    target->setVisible(false);
}

void ZFS_Pool_Stuff::setDataDescription(const QString &_xmlDesc)
{

}
QDomDocument ZFS_Pool_Stuff::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _stuff, _source, _name, _device;
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
    foreach (QString _dir, source->device->getDevicesList()) {
        if ( _dir.isEmpty() ) continue;
        _device = doc.createElement("device");
        _device.setAttribute("path", _dir);
        _source.appendChild(_device);
    };
    return doc;
}
