#include "create_pool.h"
#include "_create_storage_widgets/dir_pool_stuff.h"
#include "_create_storage_widgets/fs_pool_stuff.h"
#include "_create_storage_widgets/netfs_pool_stuff.h"
#include "_create_storage_widgets/logical_pool_stuff.h"
#include "_create_storage_widgets/disk_pool_stuff.h"
#include "_create_storage_widgets/iscsi_pool_stuff.h"
#include "_create_storage_widgets/scsi_pool_stuff.h"
#include "_create_storage_widgets/mpath_pool_stuff.h"
#include "_create_storage_widgets/rbd_pool_stuff.h"
#include "_create_storage_widgets/sheepdog_pool_stuff.h"
#include "_create_storage_widgets/gluster_pool_stuff.h"
#include "_create_storage_widgets/zfs_pool_stuff.h"

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

CreatePool::CreatePool(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr,
        QString          _connName,
        QString          _xmlFile) :
    _CreateStorage(parent, connPtrPtr, _xmlFile)
{
    //setModal(false);
    QString _title =
            QString(tr("Create Pool in [%1]"))
            .arg(_connName);
    setWindowTitle(_title);
    setUrl("http://libvirt.org/formatstorage.html");
    settingName.append("CreateStoragePool");
    settings.beginGroup(settingName);
    restoreGeometry(
                settings.value("Geometry").toByteArray());
    showAtClose->setChecked(
                settings.value("ShowAtClose").toBool() );
    settings.endGroup();
    info->addWidget(new Dir_Pool_Stuff(this, connPtrPtr));
    info->addWidget(new Fs_Pool_Stuff(this, connPtrPtr));
    info->addWidget(new NetFs_Pool_Stuff(this, connPtrPtr));
    info->addWidget(new Logical_Pool_Stuff(this, connPtrPtr));
    info->addWidget(new Disk_Pool_Stuff(this, connPtrPtr));
    info->addWidget(new iSCSI_Pool_Stuff(this, connPtrPtr));
    info->addWidget(new SCSI_Pool_Stuff(this, connPtrPtr));
    info->addWidget(new MPath_Pool_Stuff(this, connPtrPtr));
    info->addWidget(new RBD_Pool_Stuff(this, connPtrPtr));
    info->addWidget(new SheepDog_Pool_Stuff(this, connPtrPtr));
    info->addWidget(new Gluster_Pool_Stuff(this, connPtrPtr));
    info->addWidget(new ZFS_Pool_Stuff(this, connPtrPtr));

    QStringList _text, _data;
    _text = POOL_TYPE_DESC;
    _data = POOL_TYPES;
    for (int i = 0; i<_text.count(); i++) {
        type->addItem(_text.at(i), _data.at(i));
    };
    uuidLabel = new QLabel("UUID:", this);
    uuid = new QLineEdit(this);
    uuid->setPlaceholderText(
                tr("UUID will be generated if omitted"));
    uuidLayout = new QHBoxLayout();
    uuidLayout->addWidget(uuidLabel);
    uuidLayout->addWidget(uuid);
    uuidWdg = new QWidget(this);
    uuidWdg->setLayout(uuidLayout);

    commonLayout->insertWidget(
                commonLayout->count()-1, uuidWdg, -1);
    commonLayout->insertWidget(
                commonLayout->count()-1, infoWidget, -1);

    readXMLDataDescription();
}

void CreatePool::setDataDescription(const QString &_xmlDesc)
{
    //QTextStream s(stdout);
    //s << _xmlDesc << "---" << endl;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _pool;
    _pool = doc.firstChildElement("pool");
    if ( !_pool.isNull() ) {
        QString _type = _pool.attribute("type");
        int idx = type->findData(_type, Qt::UserRole);
        if ( idx<0 ) idx = 0;
        type->setCurrentIndex(idx);
        QDomNode _n = _pool.firstChild();
        while ( !_n.isNull() ) {
            QDomElement _el = _n.toElement();
            if ( !_el.isNull() ) {
                if ( _el.tagName().compare("name")==0 ) {
                    stName->setText(_el.text());
                } else if ( _el.tagName().compare("uuid")==0 ) {
                        uuid->setText(_el.text());
                };
            };
            _n = _n.nextSibling();
        };
        _Pool_Stuff *wdg =
                static_cast<_Pool_Stuff*>(info->currentWidget());
        if ( wdg!=Q_NULLPTR ) {
            wdg->setDataDescription(_xmlDesc);
        };
    };
}

/* public slots */
int CreatePool::getResult() const
{
    return result();
}
QString CreatePool::getXMLDescFileName() const
{
    QDomDocument doc, _stuff;
    QDomElement _pool, _name, _uuid;
    QDomText _text;
    _pool = doc.createElement("pool");
    QString _type = type->itemData(
                type->currentIndex(),
                Qt::UserRole)
            .toString();
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

    _Pool_Stuff *wdg =
            static_cast<_Pool_Stuff*>(info->currentWidget());
    if ( wdg!=Q_NULLPTR ) {
        _stuff = wdg->getDataDocument();
        QDomNodeList list =
                _stuff.firstChildElement("stuff").childNodes();
        /*
         * current DomNode is removed to root-element
         * but NULL-elemens not removed
         * therefore keep to seek on not-NULL next element
         */
        int j = 0;
        const int count = list.length();
        for (int i=0; i<count;i++) {
            //qDebug()<<list.item(j).nodeName()<<i;
            if (!list.item(j).isNull())
                _pool.appendChild(list.item(j));
            else ++j;
        };
    };

    bool read = xml->open();
    if (read) xml->write(doc.toByteArray(4).data());
    xml->close();
    return xml->fileName();
}
