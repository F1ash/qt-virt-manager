#include "_pool_stuff.h"

_Pool_Stuff::_Pool_Stuff(QWidget *parent, virConnectPtr _conn) :
    QWidget(parent)
{
    source = new _Storage_Source(this, _conn);
    target = new _Storage_Target(this, _conn);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(source);
    commonLayout->addWidget(target);
    commonLayout->insertStretch(2, -1);
    setLayout(commonLayout);
    target->pathWdg->setVisible(true);
}

/* public slots */
QDomDocument _Pool_Stuff::getStorageXMLDesc() const
{
    return QDomDocument();
    /*
    QString _poolType = type->itemData(
                type->currentIndex(),
                Qt::UserRole)
            .toString();
    QDomDocument doc;
    QDomText _text;
    QDomElement _pool, _name, _target, _auth, _secret,
            _source, _format, _permissions, _owner,
            _group, _mode, _label, _path, _dir, _vendor,
            _product, _host, _device;
    _pool = doc.createElement("pool");
    _name = doc.createElement("name");
    _text = doc.createTextNode(stName->text());
    _name.appendChild(_text);
    _pool.appendChild(_name);
    _source = doc.createElement("source");
    if ( source->auth->auth->isChecked() &&
         (_poolType=="iscsi" || _poolType=="rbd") ) {
        _auth = doc.createElement("auth");
        _auth.setAttribute(
                    "type",
                    (_poolType=="rbd")? "ceph" : "chap");
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
    if ( !source->dir->text().isEmpty() ) {
        _dir = doc.createElement("dir");
        _dir.setAttribute(
                    "path",
                    source->dir->text());
        _source.appendChild(_dir);
    };
    if ( !source->named->text().isEmpty() ) {
        _name = doc.createElement("name");
        _text = doc.createTextNode(source->named->text());
        _name.appendChild(_text);
        _source.appendChild(_name);
    };
    if ( source->format->count()!=0 &&
         source->format->currentText()!="default" ) {
        _format = doc.createElement("format");
        _format.setAttribute(
                    "type",
                    target->format->currentText().toLower());
        _source.appendChild(_format);
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
    _pool.appendChild(_source);
    _target = doc.createElement("target");
    if ( !target->path->text().isEmpty() ) {
        _path = doc.createElement("path");
        _text =doc.createTextNode(
                    target->path->text());
        _path.appendChild(_text);
        _target.appendChild(_path);
    };
    if ( target->usePerm->isChecked() ) {
        _permissions = doc.createElement("permissions");
        _owner = doc.createElement("owner");
        _text = doc.createTextNode(target->owner->text());
        _owner.appendChild(_text);
        _permissions.appendChild(_owner);
        _group = doc.createElement("group");
        _text = doc.createTextNode(target->group->text());
        _group.appendChild(_text);
        _permissions.appendChild(_group);
        _mode = doc.createElement("mode");
        _text = doc.createTextNode(target->mode->text());
        _mode.appendChild(_text);
        _permissions.appendChild(_mode);
        _label = doc.createElement("label");
        _text = doc.createTextNode(target->label->text());
        _label.appendChild(_text);
        _permissions.appendChild(_label);
        _target.appendChild(_permissions);
    };
    if ( target->format->currentText()!="default" ) {
        _format = doc.createElement("format");
        _format.setAttribute(
                    "type",
                    target->format->currentText().toLower());
        _target.appendChild(_format);
    };
    _pool.appendChild(_target);
    _pool.setAttribute("type", _poolType);
    doc.appendChild(_pool);
    qDebug()<<doc.toString();
    */
}
