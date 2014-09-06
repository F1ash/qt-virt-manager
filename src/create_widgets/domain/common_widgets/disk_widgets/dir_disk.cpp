#include "dir_disk.h"

Dir_Disk::Dir_Disk(
        QWidget *parent,
        virConnectPtr conn) :
    _QWidget(parent, conn)
{
    browse = new QPushButton("Directory:", this);
    path = new QLineEdit(this);
    path->setPlaceholderText("/var/lib/xen/images/");
    baseLayout = new QGridLayout();
    baseLayout->addWidget(browse, 0, 0);
    baseLayout->addWidget(path, 0, 1);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    startupPolicy = new _StartupPolicy(this);
    devType = new _Device(this);
    target = new _Target(this);
    readOnly = new _ReadOnly(this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(startupPolicy);
    commonLayout->addWidget(devType);
    commonLayout->addWidget(target);
    commonLayout->addWidget(readOnly);
    setLayout(commonLayout);
    connect(browse, SIGNAL(clicked()),
            this, SLOT(setDirPath()));
}

/* public slots */
QDomDocument Dir_Disk::getDevDocument() const
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

/* private slots */
void Dir_Disk::setDirPath()
{
    QString dirPath = QFileDialog::getExistingDirectory(
                this,
                "Directory Path",
                "/");
    if ( !dirPath.isEmpty() ) path->setText(dirPath);
}
