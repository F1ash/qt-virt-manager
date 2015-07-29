#include "create_pool.h"

/*
 * http://libvirt.org/formatstorage.html
 */

#define POOL_TYPE_DESC QStringList()\
    <<"Directory backend"<<"Local filesystem backend"\
    <<"Network filesystem backend"<<"Logical backend"\
    <<"Disk backend"<<"iSCSI backend"<<"SCSI backend"\
    <<"Multipath backend"<<"RBD (RADOS Block Device) backend"\
    <<"Sheepdog backend"<<"Gluster backend"<<"ZFS backend"

#define POOL_TYPES QStringList()\
    <<"dir"<<"fs"<<"netfs"<<"logical"\
    <<"disk"<<"iscsi"<<"scsi"\
    <<"mpath"<<"rbd"<<"sheepdog"\
    <<"gluster"<<"zfs"

CreatePool::CreatePool(QWidget *parent, virConnectPtr _conn) :
    _CreateStorage(parent)
{
    setUrl("http://libvirt.org/formatstorage.html");
    settingName.append("CreateStoragePool");
    settings.beginGroup(settingName);
    restoreGeometry(settings.value("Geometry").toByteArray());
    showAtClose->setChecked( settings.value("ShowAtClose").toBool() );
    settings.endGroup();
    info->addWidget(new Dir_Pool_Stuff(this));
    info->addWidget(new Fs_Pool_Stuff(this));
    info->addWidget(new NetFs_Pool_Stuff(this));
    info->addWidget(new Logical_Pool_Stuff(this));
    info->addWidget(new Disk_Pool_Stuff(this));
    info->addWidget(new iSCSI_Pool_Stuff(this, _conn));
    info->addWidget(new SCSI_Pool_Stuff(this));
    info->addWidget(new MPath_Pool_Stuff(this));
    info->addWidget(new RBD_Pool_Stuff(this, _conn));
    info->addWidget(new SheepDog_Pool_Stuff(this));
    info->addWidget(new Gluster_Pool_Stuff(this));
    info->addWidget(new ZFS_Pool_Stuff(this));

    QStringList _text, _data;
    _text = POOL_TYPE_DESC;
    _data = POOL_TYPES;
    for (int i = 0; i<_text.count(); i++) {
        type->addItem(_text.at(i), _data.at(i));
    };
    uuidLabel = new QLabel("UUID:", this);
    uuid = new QLineEdit(this);
    uuid->setPlaceholderText("a UUID will be generated if omitted");
    uuidLayout = new QHBoxLayout();
    uuidLayout->addWidget(uuidLabel);
    uuidLayout->addWidget(uuid);
    uuidWdg = new QWidget(this);
    uuidWdg->setLayout(uuidLayout);

    commonLayout->insertWidget(commonLayout->count()-1, uuidWdg, -1);
    commonLayout->insertWidget(commonLayout->count()-1, infoWidget, -1);
}

/* public slots */
QString CreatePool::getStorageXMLDescFileName() const
{
    QDomDocument doc, _stuff;
    QDomElement _pool, _name, _uuid;
    QDomText _text;
    _pool = doc.createElement("pool");
    QString _type = type->itemData(type->currentIndex(), Qt::UserRole).toString();
    _pool.setAttribute("type", _type);
    doc.appendChild(_pool);
    _name = doc.createElement("name");
    _text = doc.createTextNode(stName->text());
    _name.appendChild(_text);
    _pool.appendChild(_name);
    if ( !uuid->text().isEmpty() ) {
        _uuid = doc.createElement("uuid");
        _text = doc.createTextNode(uuid->text());
        _uuid.appendChild(_text);
        _pool.appendChild(_uuid);
    };

    _Pool_Stuff *wdg = static_cast<_Pool_Stuff*>(info->currentWidget());
    _stuff = wdg->getStorageXMLDesc();
    QDomNodeList list = _stuff.firstChildElement("stuff").childNodes();
    /*
     * current DomNode is removed to root-element
     * but NULL-elemens not removed
     * therefore keep to seek on not-NULL next element
     */
    uint j = 0;
    uint count = list.length();
    for (int i=0; i<count;i++) {
        //qDebug()<<list.item(j).nodeName()<<i;
        if (!list.item(j).isNull()) _pool.appendChild(list.item(j));
        else ++j;
    };

    bool read = xml->open();
    if (read) xml->write(doc.toByteArray(4).data());
    xml->close();
    return xml->fileName();
}
