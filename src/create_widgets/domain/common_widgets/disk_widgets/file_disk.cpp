#include "file_disk.h"

File_Disk::File_Disk(
        QWidget *parent,
        virConnectPtr conn) :
    _Disk(parent, conn)
{
    browse = new QPushButton("File:", this);
    path = new QLineEdit(this);
    path->setPlaceholderText("/var/lib/xen/images/fv0");

    baseLayout->addWidget(browse, 0, 0);
    baseLayout->addWidget(path, 0, 1);

    secLabels->setVisible(true);

    connect(browse, SIGNAL(clicked()),
            this, SLOT(setFilePath()));
    // dataChanged connections
    connect(secLabels, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument File_Disk::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _source, _target, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("disk");

    _source = doc.createElement("source");
    _source.setAttribute("file", path->text());
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

    if ( secLabels->isUsed() ) {
        QDomNodeList _l = secLabels->getDataDocument()
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

    _devDesc.setAttribute("type", "file");
    _devDesc.setAttribute("device", devType->getDeviceType());
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void File_Disk::setDataDescription(QString &xmlDesc)
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
    _attr = _source.attribute("file");
    path->setText(_attr);
    _attr = _target.attribute("dev");
    target->devName->setText(_attr);
    readOnly->readOnly->setChecked( !_readOnly.isNull() );
    secLabels->setUsage( !_secLabel.isNull() );
    if ( !_secLabel.isNull() ) {
        QDomDocument _doc;
        _doc.setContent(QString());
        _doc.appendChild(_device);
        _device.setTagName("domain");
        QString _xmlDesc = _doc.toString();
        secLabels->readXMLDesciption(_xmlDesc);
    };
}

/* private slots */
void File_Disk::setFilePath()
{
    QString filePath = QFileDialog::getOpenFileName(
                this,
                "File Path",
                "/");
    if ( !filePath.isEmpty() ) path->setText(filePath);
}
