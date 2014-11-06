#include "dir_disk.h"

Dir_Disk::Dir_Disk(
        QWidget *parent,
        virConnectPtr conn) :
    _Disk(parent, conn)
{
    browse = new QPushButton("Directory:", this);
    path = new QLineEdit(this);
    path->setPlaceholderText("/var/lib/xen/images/");

    baseLayout->addWidget(browse, 0, 0);
    baseLayout->addWidget(path, 0, 1);

    connect(browse, SIGNAL(clicked()),
            this, SLOT(setDirPath()));
    // dataChanged connections
    connect(path, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument Dir_Disk::getDataDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _target, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("disk");

    _source = doc.createElement("source");
    _source.setAttribute("dir", path->text());
    if ( startupPolicy->isUsed() )
        _source.setAttribute("startupPolicy",
                             startupPolicy->getStartupPolicy());
    _devDesc.appendChild(_source);

    _target = doc.createElement("target");
    TargetAttrs l = target->getTargetAttrs();
    foreach (QString key, l.keys()) {
        if ( !key.isEmpty() )
        _target.setAttribute(key, l.value(key));
    };
    _devDesc.appendChild(_target);

    if ( readOnly->state() ) {
        QDomElement _readOnly = doc.createElement("readonly");
        _devDesc.appendChild(_readOnly);
    }

    _devDesc.setAttribute("type", "dir");
    _devDesc.setAttribute("device", devType->getDeviceType());
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void Dir_Disk::setDataDescription(QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _source, _target,
            _readOnly, _secLabel, _driver;
    _device = doc.firstChildElement("device")
            .firstChildElement("disk");
    _source = _device.firstChildElement("source");
    _target = _device.firstChildElement("target");
    _readOnly = _device.firstChildElement("readonly");
    _secLabel = _device.firstChildElement("seclabel");
    _driver = _device.firstChildElement("driver");
    QString _attr;
    int idx;
    _attr = _source.attribute("dir");
    path->setText(_attr);
    _attr = _source.attribute("device");
    devType->setDeviceType(_attr);
    if ( _source.hasAttribute("startupPolicy") ) {
        startupPolicy->setUsage(true);
        _attr = _source.attribute("startupPolicy");
        idx = startupPolicy->findPolicyIndex(_attr);
        startupPolicy->setPolicyIndex( (idx<0)? 0:idx );
    } else
        startupPolicy->setUsage(false);
    _attr = _target.attribute("dev");
    target->devName->setText(_attr);
    _attr = _target.attribute("bus");
    idx = target->bus->findText(_attr, Qt::MatchContains);
    target->bus->setCurrentIndex( (idx<0)? 0:idx );
    if ( _target.hasAttribute("tray") ) {
        target->trayLabel->setChecked(true);
        _attr = _target.attribute("tray");
        idx = target->tray->findText(_attr, Qt::MatchContains);
        target->tray->setCurrentIndex( (idx<0)? 0:idx );
    } else
        target->trayLabel->setChecked(false);
    if ( _target.hasAttribute("removable") ) {
        target->removableLabel->setChecked(true);
        _attr = _target.attribute("removable");
        idx = target->removable->findText(_attr, Qt::MatchContains);
        target->removable->setCurrentIndex( (idx<0)? 0:idx );
    } else
        target->removableLabel->setChecked(false);
    readOnly->readOnly->setChecked( !_readOnly.isNull() );
}

/* private slots */
void Dir_Disk::setDirPath()
{
    QString dirPath = QFileDialog::getExistingDirectory(
                this,
                "Directory Path",
                "/");
    if ( !dirPath.isEmpty() ) path->setText(dirPath);
}
