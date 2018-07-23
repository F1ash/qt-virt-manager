#include "create_volume.h"

pooHelperThread::pooHelperThread(
        QObject *parent,
        virConnectPtr *connPtrPtr,
        QString _name) :
    _VirtThread(parent, connPtrPtr), name(_name)
{

}
void pooHelperThread::run()
{
    if ( Q_NULLPTR==ptr_ConnPtr || Q_NULLPTR==*ptr_ConnPtr ) {
        emit ptrIsNull();
        return;
    };
    if ( virConnectRef(*ptr_ConnPtr)<0 ) {
        sendConnErrors();
        return;
    };
    // something data reading
    pool = virStoragePoolLookupByName(
            *ptr_ConnPtr, name.toUtf8().data());
    QDomDocument doc;
    doc.setContent(
                QString(
                    virStoragePoolGetXMLDesc(
                        pool,
                        VIR_STORAGE_XML_INACTIVE))
                );
    type = doc.firstChildElement("pool").attribute("type");
    if ( virConnectClose(*ptr_ConnPtr)<0 )
        sendConnErrors();
}

/*
 * http://libvirt.org/formatstorage.html
 */

CreateVolume::CreateVolume(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr,
        QString          _poolName,
        QString          _xmlFile) :
    _CreateStorage(parent, connPtrPtr, _xmlFile)
{
    QString _title =
            QString(tr("Create Volume in [%1] Pool"))
            .arg(_poolName);
    setWindowTitle(_title);
    setUrl("http://libvirt.org/formatstorage.html");
    settingName.append("CreateStorageVolume");
    settings.beginGroup(settingName);
    restoreGeometry(
                settings.value("Geometry").toByteArray());
    showAtClose->setChecked(
                settings.value("ShowAtClose").toBool() );
    settings.endGroup();
    suff->setVisible(true);
    allocLabel = new QComboBox(this);
    allocLabel->addItem("Allocation (KiB):", "KiB");
    allocLabel->addItem("Allocation (MiB):", "MiB");
    allocLabel->addItem("Allocation (GiB):", "GiB");
    allocLabel->addItem("Allocation (TiB):", "TiB");
    allocLabel->addItem("Allocation (EiB):", "EiB");
    capLabel = new QComboBox(this);
    capLabel->addItem("Capacity (KiB):", "KiB");
    capLabel->addItem("Capacity (MiB):", "MiB");
    capLabel->addItem("Capacity (GiB):", "GiB");
    capLabel->addItem("Capacity (TiB):", "TiB");
    capLabel->addItem("Capacity (EiB):", "EiB");
    allocation = new QSpinBox(this);
    allocation->setRange(0, 1024000000); // 1024Mib
    capacity = new QSpinBox(this);
    capacity->setRange(0, 1024000000); // 1024Mib
    sizeLayout = new QGridLayout();
    sizeLayout->addWidget(allocLabel, 0, 0);
    sizeLayout->addWidget(allocation, 0, 1);
    sizeLayout->addWidget(capLabel, 1, 0);
    sizeLayout->addWidget(capacity, 1, 1);
    sizeWdg = new QWidget(this);
    sizeWdg->setLayout(sizeLayout);

    hlpThread = new pooHelperThread(
                this, connPtrPtr, _poolName);
    connect(hlpThread, SIGNAL(finished()),
            this, SLOT(initData()));
    connect(hlpThread, SIGNAL(errorMsg(const QString&, const uint)),
            this, SIGNAL(errorMsg(const QString&)));
    hlpThread->start();
}

void CreateVolume::setDataDescription(const QString &_xmlDesc)
{
    //QTextStream s(stdout);
    //s << _xmlDesc << "---" << endl;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _vol;
    _vol = doc.firstChildElement("volume");
    if ( !_vol.isNull() ) {
        QString _type = _vol.attribute("type");
        // type set in initData() method
        QDomNode _n = _vol.firstChild();
        while ( !_n.isNull() ) {
            QDomElement _el = _n.toElement();
            if ( !_el.isNull() ) {
                if ( _el.tagName().compare("name")==0 ) {
                    stName->setText(_el.text());
                } else if ( _el.tagName().compare("allocation")==0 ) {
                    QString _unit = _el.attribute("unit", "bytes");
                    allocation->setValue(
                                convertNiBtoMiB(_el.text().toULongLong(), _unit));
                    int idx = allocLabel->findData("MiB", Qt::UserRole);
                    if (idx <0 ) idx = 0;
                    allocLabel->setCurrentIndex(idx);
                } else if ( _el.tagName().compare("capacity")==0 ) {
                    QString _unit = _el.attribute("unit", "bytes");
                    capacity->setValue(
                                convertNiBtoMiB(_el.text().toULongLong(), _unit));
                    int idx = capLabel->findData("MiB", Qt::UserRole);
                    if (idx <0 ) idx = 0;
                    capLabel->setCurrentIndex(idx);
                } else if ( _el.tagName().compare("target")==0 ) {
                    QDomNode _n1 = _el.firstChild();
                    while ( !_n1.isNull() ) {
                        QDomElement _el1 = _n1.toElement();
                        if ( !_el1.isNull() ) {
                            if ( _el1.tagName().compare("path")==0 ) {
                                target->path->setText(_el1.text());
                            } else if ( _el1.tagName().compare("format")==0 ) {
                                target->setVolumeFormat(
                                            _el1.attribute("type", "default"));
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
                            } else if ( _el1.tagName().compare("encryption")==0 ) {
                                target->encrypt->setUsage(true);
                                target->encrypt->setFormat(
                                            _el1.attribute("format", "default"));
                                QDomElement _secret = _el1.firstChildElement("secret");
                                if ( !_secret.isNull() ) {
                                    target->encrypt->setAutoSecretUsage(true);
                                    target->encrypt->setSecretUUID(
                                                _secret.attribute("uuid"));
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

/* public slots */
QString CreateVolume::getXMLDescFileName() const
{
    QDomDocument doc;
    QDomText _text;
    QDomElement _volume, _name, _target,
            _source, _allocation, _capacity,
            _format, _permissions, _owner,
            _group, _mode, _label, _encrypt, _secret;
    _volume = doc.createElement("volume");
    _name = doc.createElement("name");
    _text = doc.createTextNode(
                QString("%1.img").arg(stName->text()));
    _name.appendChild(_text);
    _volume.appendChild(_name);
    _allocation = doc.createElement("allocation");
    _text = doc.createTextNode(allocation->text());
    _allocation.setAttribute(
                "unit",
                allocLabel->itemData(
                    allocLabel->currentIndex(),
                    Qt::UserRole).toString());
    _allocation.appendChild(_text);
    _volume.appendChild(_allocation);
    _capacity = doc.createElement("capacity");
    _text = doc.createTextNode(capacity->text());
    _capacity.setAttribute(
                "unit",
                capLabel->itemData(
                    capLabel->currentIndex(),
                    Qt::UserRole).toString());
    _capacity.appendChild(_text);
    _volume.appendChild(_capacity);
    _target = doc.createElement("target");
    if ( target->usePerm->isChecked() ||
         target->format->currentText().compare("default")!=0 ) {
        if ( target->format->currentText().compare("default")!=0 ) {
            _format = doc.createElement("format");
            _format.setAttribute(
                        "type",
                        target->format->currentText().toLower());
            _target.appendChild(_format);
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
    };
    if ( target->encrypt->isUsed() ) {
        _encrypt = doc.createElement("encryption");
        _encrypt.setAttribute(
                    "format",
                    target->encrypt->getFormat());
        if ( !target->encrypt->AutoSecretIsUsed() ) {
            _secret = doc.createElement("secret");
            _secret.setAttribute("type", "passphrase");
            _secret.setAttribute("uuid", target->encrypt->getSecretUUID());
            _encrypt.appendChild(_secret);
        };
        _target.appendChild(_encrypt);
    };
    if ( !_target.isNull() ) _volume.appendChild(_target);
    doc.appendChild(_volume);
    //qDebug()<<doc.toString();

    bool read = xml->open();
    if (read) xml->write(doc.toByteArray(4).data());
    xml->close();
    return xml->fileName();
}

/* private slots */
void CreateVolume::initData()
{
    // set pool type, but need volume type
    type->addItem(hlpThread->type);

    //source = new _Storage_Source(this);
    target = new _Storage_Target(this, ptr_ConnPtr, hlpThread->type);
    target->formatWdg->setVisible(true);
    target->encrypt->setVisible(true);
    target->pathWdg->setVisible(true);

    infoStuffLayout = new QVBoxLayout();
    //infoStuffLayout->addWidget(source);
    infoStuffLayout->addWidget(target);
    infoStuffLayout->addStretch(-1);
    infoStuff = new QWidget(this);
    infoStuff->setLayout(infoStuffLayout);
    info->addWidget(infoStuff);

    commonLayout->insertWidget(
                commonLayout->count()-1, sizeWdg);
    commonLayout->insertWidget(
                commonLayout->count()-1, infoWidget, -1);
    connect(allocLabel, SIGNAL(currentIndexChanged(int)),
            capLabel, SLOT(setCurrentIndex(int)));
    connect(capLabel, SIGNAL(currentIndexChanged(int)),
            allocLabel, SLOT(setCurrentIndex(int)));
    allocLabel->setCurrentIndex(1);
    //capLabel->setCurrentIndex(1);

    readXMLDataDescription();
}
