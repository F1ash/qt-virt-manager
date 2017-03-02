#include "scsi_pool_stuff.h"

/*
 * http://libvirt.org/storage.html#StorageBackendSCSI
 */

SCSI_Pool_Stuff::SCSI_Pool_Stuff(QWidget *parent) :
    _Pool_Stuff(parent)
{
    source->adapter->setVisible(true);
    source->vendorLabel->setVisible(true);
    source->productLabel->setVisible(true);
    source->vendor->setVisible(true);
    source->product->setVisible(true);
    target->path->setPlaceholderText("/dev/disk/{by-path, by-id}");
}

/* public slots */
QDomDocument SCSI_Pool_Stuff::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _stuff, _source, _adapter, _vendor, _product,
            _target, _path, _perm, _encrypt;
    QDomText _text;
    _stuff = doc.createElement("stuff");
    doc.appendChild(_stuff);
    _source = doc.createElement("source");
    _stuff.appendChild(_source);

    _adapter = doc.createElement("adapter");
    _source.appendChild(_adapter);
    if ( source->adapter->type->currentText()=="scsi_host" ) {
        _adapter.setAttribute("type", "scsi_host");
        if ( source->adapter->usePrntAddr->isChecked() ) {
            QDomElement _address, _parentaddr;
            _parentaddr = doc.createElement("parentaddr");
            _parentaddr.setAttribute(
                        "unique_id",
                        source->adapter->adapterPrntID->text());
            _adapter.appendChild(_parentaddr);
            _address = doc.createElement("address");
            _parentaddr.appendChild(_address);
            AttrList l = source->adapter->addr->getAttrList();
            foreach (QString key, l.keys()) {
                if ( !key.isEmpty() )
                    _address.setAttribute(key, l.value(key));
            };
        } else {
            _adapter.setAttribute(
                        "name",
                        source->adapter->name->text());
        };
    } else if ( source->adapter->type->currentText()=="fc_host" ) {
        if ( !source->adapter->name->text().isEmpty() )
            _adapter.setAttribute("name", source->adapter->name->text());
        _adapter.setAttribute("type", "fc_host");
        _adapter.setAttribute(
                    "parent",
                    source->adapter->adapterPrnt->text());
        _adapter.setAttribute(
                    "wwnn",
                    source->adapter->wwnn->text());
        _adapter.setAttribute(
                    "wwpn",
                    source->adapter->wwpn->text());
    };

    if ( !source->vendor->text().isEmpty() ) {
        _vendor = doc.createElement("vendor");
        _vendor.setAttribute(
                    "name",
                    source->vendor->text());
        _source.appendChild(_vendor);
    };
    if ( !source->product->text().isEmpty() ) {
        _product = doc.createElement("product");
        _product.setAttribute(
                    "name",
                    source->product->text());
        _source.appendChild(_product);
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
        _encrypt = doc.createElement("encryption");
        _target.appendChild(_encrypt);
        _encrypt.setAttribute(
                    "format",
                    target->encrypt->getFormat());
    };
    return doc;
}
