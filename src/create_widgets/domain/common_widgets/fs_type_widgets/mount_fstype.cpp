#include "mount_fstype.h"

MountFsType::MountFsType(QWidget *parent, QString _type) :
    _FsType(parent, _type)
{
    source->setPlaceholderText(tr("Source host directory"));
    target->setPlaceholderText(tr("Target guest directory"));
    connect(sourceLabel, SIGNAL(clicked()),
            this, SLOT(getSourcePath()));
    // dataChanged connections
    connect(source, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(target, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(readOnly, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(accessMode, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(driver, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(wrPolicy, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(format, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument MountFsType::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _source, _target, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("filesystem");
    if ( connType.compare("qemu")==0
         && accessMode->currentText().compare("default")!=0 ) {
        _devDesc.setAttribute("accessmode", accessMode->currentText());
    };
    if ( connType.compare("qemu")==0
         && driver->currentText().compare("default")!=0 ) {
        QDomElement _driver = doc.createElement("driver");
        _devDesc.appendChild(_driver);
        _driver.setAttribute("type", driver->currentText());
        if ( wrPolicy->currentText().compare("default")!=0 )
            _driver.setAttribute("wrpolicy", wrPolicy->currentText());
    } else if ( connType.compare("lxc")==0
                && driver->currentText().compare("default")!=0 ) {
        QDomElement _driver = doc.createElement("driver");
        _devDesc.appendChild(_driver);
        if ( wrPolicy->currentText().compare("default")!=0 )
            _driver.setAttribute("wrpolicy", wrPolicy->currentText());
        _driver.setAttribute("type", driver->currentText());
        _driver.setAttribute("format", format->currentText());
    }
    _source = doc.createElement("source");
    _source.setAttribute("dir", source->text());
    _devDesc.appendChild(_source);
    _target = doc.createElement("target");
    _target.setAttribute("dir", target->text());
    _devDesc.appendChild(_target);
    if ( readOnly->isChecked() ) {
        QDomElement _readOnly = doc.createElement("readonly");
        _devDesc.appendChild(_readOnly);
    };
    _devDesc.setAttribute("type", "mount");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void MountFsType::setDataDescription(const QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    int idx;
    doc.setContent(_xmlDesc);
    QDomElement _source, _target, _driver,
            _readOnly, _device;
    _device = doc.firstChildElement("device")
            .firstChildElement("filesystem");
    _driver = _device.firstChildElement("driver");
    _source = _device.firstChildElement("source");
    _target = _device.firstChildElement("target");
    _readOnly = _device.firstChildElement("readonly");
    if ( connType.compare("qemu")==0 ) {
        idx = accessMode->findText(
                    _device.attribute("accessmode"), Qt::MatchContains);
        accessMode->setCurrentIndex( (idx<0)? 0:idx );
    };
    if ( connType.compare("qemu")==0 && !_driver.isNull() ) {
        idx = wrPolicy->findText(
                    _driver.attribute("wrpolicy"), Qt::MatchContains);
        wrPolicy->setCurrentIndex( (idx<0)? 0:idx );
        idx = driver->findText(
                    _driver.attribute("type"), Qt::MatchContains);
        driver->setCurrentIndex( (idx<0)? 0:idx );
    } else if ( connType.compare("lxc")==0 && !_driver.isNull() ) {
        idx = wrPolicy->findText(
                    _driver.attribute("wrpolicy"), Qt::MatchContains);
        wrPolicy->setCurrentIndex( (idx<0)? 0:idx );
        idx = driver->findText(
                    _driver.attribute("type"), Qt::MatchContains);
        driver->setCurrentIndex( (idx<0)? 0:idx );
        idx = format->findText(
                    _driver.attribute("format"), Qt::MatchContains);
        format->setCurrentIndex( (idx<0)? 0:idx );
    } else if ( _driver.isNull() )
        driver->setCurrentIndex(0);
    source->setText(_source.attribute("dir"));
    target->setText(_target.attribute("dir"));
    readOnly->setChecked( !_readOnly.isNull() );
}

/* private slots */
void MountFsType::getSourcePath()
{
    QString dirName = QFileDialog::getExistingDirectory(
                this,
                tr("Source Path"),
                "~");
    if ( !dirName.isEmpty() ) source->setText(dirName);
}
