#include "block_disk.h"

Block_Disk::Block_Disk(
        QWidget *parent,
        virConnectPtr conn) :
    _Disk(parent, conn)
{
    browse = new QPushButton("Block device:", this);
    path = new QLineEdit(this);
    path->setPlaceholderText("/dev/sda");

    baseLayout->addWidget(browse, 0, 0);
    baseLayout->addWidget(path, 0, 1);

    secLabel->setVisible(true);

    connect(browse, SIGNAL(clicked()),
            this, SLOT(setDevPath()));
}

/* public slots */
QDomDocument Block_Disk::getDataDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _target, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("disk");

    _source = doc.createElement("source");
    _source.setAttribute("dev", path->text());
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

    if ( secLabel->isUsed() ) {
        QDomNodeList _l = secLabel->getDataDocument()
                .firstChildElement("data")
                .childNodes();
        uint j = 0;
        uint count = _l.length();
        for (uint i=0; i<count;i++) {
            //qDebug()<<_l.item(j).nodeName()<<i;
            if (!_l.item(j).isNull()) _devDesc.appendChild(_l.item(j));
            else ++j;
        };
    };

    if ( readOnly->state() ) {
        QDomElement _readOnly = doc.createElement("readonly");
        _devDesc.appendChild(_readOnly);
    }

    _devDesc.setAttribute("type", "block");
    _devDesc.setAttribute("device", devType->getDeviceType());
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}

/* private slots */
void Block_Disk::setDevPath()
{
    QString filePath = QFileDialog::getOpenFileName(
                this,
                "Block Device Path",
                "/dev");
    if ( !filePath.isEmpty() ) path->setText(filePath);
}
