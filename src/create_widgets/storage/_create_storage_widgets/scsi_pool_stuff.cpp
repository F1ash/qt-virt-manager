#include "scsi_pool_stuff.h"

/*
 * http://libvirt.org/storage.html#StorageBackendSCSI
 */

SCSI_Pool_Stuff::SCSI_Pool_Stuff(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    _Pool_Stuff(parent, connPtrPtr)
{
    source->adapter->setVisible(true);
    source->vendorLabel->setVisible(true);
    source->productLabel->setVisible(true);
    source->vendor->setVisible(true);
    source->product->setVisible(true);
    target->path->setPlaceholderText("/dev/disk/{by-path, by-id}");
}

void SCSI_Pool_Stuff::setDataDescription(const QString &_xmlDesc)
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
                    QDomElement _el1 = _el.firstChildElement("adapter"); // May only occur once
                    if ( !_el1.isNull() ) {
                        QString _type = _el1.attribute("type", "scsi_host");
                        int idx = source->adapter->type->findText(_type);
                        if ( idx<0 ) idx = 0;
                        source->adapter->type->setCurrentIndex(idx);
                        if        ( _type.startsWith("scsi_host") ) {
                            QDomElement _el2 = _el1.firstChildElement("parentaddr");
                            if ( !_el2.isNull() ) {
                                source->adapter->usePrntAddr->setChecked(true);
                                source->adapter->adapterPrntID->setText(
                                            _el2.attribute("unique_id"));
                                QDomElement _el3 = _el2.firstChildElement("address");
                                if ( !_el3.isNull() ) {
                                    source->adapter->addr->domain->setText(
                                                _el3.attribute("domain"));
                                    source->adapter->addr->bus->setText(
                                                _el3.attribute("bus"));
                                    source->adapter->addr->slot->setText(
                                                _el3.attribute("slot"));
                                    source->adapter->addr->function->setValue(
                                                _el3.attribute("function").toInt());
                                };
                            } else {
                                source->adapter->name->setText(
                                            _el1.attribute("name"));
                            };
                        } else if ( _type.startsWith("fc_host") ) {
                            source->adapter->name->setText(
                                        _el1.attribute("name"));
                            source->adapter->adapterPrnt->setText(
                                        _el1.attribute("parent"));
                            source->adapter->wwnn->setText(
                                        _el1.attribute("wwnn"));
                            source->adapter->wwpn->setText(
                                        _el1.attribute("wwpn"));
                        };
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
QDomDocument SCSI_Pool_Stuff::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _stuff, _source, _adapter, _vendor, _product,
            _target, _path, _perm;
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
    return doc;
}
