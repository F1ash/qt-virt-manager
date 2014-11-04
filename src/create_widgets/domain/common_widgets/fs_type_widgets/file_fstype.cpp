#include "file_fstype.h"

FileFsType::FileFsType(QWidget *parent, QString _type) :
    _FsType(parent, _type)
{
    source->setPlaceholderText("Source host file");
    target->setPlaceholderText("Target guest directory");
    connect(sourceLabel, SIGNAL(clicked()),
            this, SLOT(getSourcePath()));
    // dataChanged connections
    connect(source, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(target, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(readOnly, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    // Currently this only works with type='mount' for the QEMU/KVM driver.
    //connect(accessMode, SIGNAL(currentIndexChanged(int)),
    //        this, SLOT(stateChanged()));
    connect(driver, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(wrPolicy, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(format, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument FileFsType::getDataDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _target, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("filesystem");
    // Currently this only works with type='mount' for the QEMU/KVM driver.
    //if ( connType=="qemu" && accessMode->currentText()!="default" ) {
    //    _devDesc.setAttribute("accessmode", accessMode->currentText());
    //};
    if ( connType=="qemu" && driver->currentText()!="default" ) {
        QDomElement _driver = doc.createElement("driver");
        _devDesc.appendChild(_driver);
        _driver.setAttribute("type", driver->currentText());
        if ( wrPolicy->currentText()!="default" )
            _driver.setAttribute("wrpolicy", wrPolicy->currentText());
    } else if ( connType=="lxc" && driver->currentText()!="default" ) {
        QDomElement _driver = doc.createElement("driver");
        _devDesc.appendChild(_driver);
        if ( wrPolicy->currentText()!="default" )
            _driver.setAttribute("wrpolicy", wrPolicy->currentText());
        _driver.setAttribute("type", driver->currentText());
        _driver.setAttribute("format", format->currentText());
    };
    _source = doc.createElement("source");
    _source.setAttribute("file", source->text());
    _devDesc.appendChild(_source);
    _target = doc.createElement("target");
    _target.setAttribute("dir", target->text());
    _devDesc.appendChild(_target);
    if ( readOnly->isChecked() ) {
        QDomElement _readOnly = doc.createElement("readonly");
        _devDesc.appendChild(_readOnly);
    };
    _devDesc.setAttribute("type", "file");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void FileFsType::setDataDescription(QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    int idx;
    doc.setContent(xmlDesc);
    QDomElement _source, _target, _driver,
            _readOnly, _device;
    _device = doc.firstChildElement("device")
            .firstChildElement("filesystem");
    _driver = _device.firstChildElement("driver");
    _source = _device.firstChildElement("source");
    _target = _device.firstChildElement("target");
    _readOnly = _device.firstChildElement("readonly");
    /* Currently this only works with type='mount' for the QEMU/KVM driver.
    if ( connType=="qemu" ) {
        idx = accessMode->findText(
                    _device.attribute("accessmode"), Qt::MatchContains);
        accessMode->setCurrentIndex( (idx<0)? 0:idx );
    };
    */
    if ( connType=="qemu" && !_driver.isNull() ) {
        idx = driver->findText(
                    _driver.attribute("type"), Qt::MatchContains);
        driver->setCurrentIndex( (idx<0)? 0:idx );
        idx = wrPolicy->findText(
                    _driver.attribute("wrpolicy"), Qt::MatchContains);
        wrPolicy->setCurrentIndex( (idx<0)? 0:idx );
    } else if ( connType=="lxc" && !_driver.isNull() ) {
        idx = driver->findText(
                    _driver.attribute("type"), Qt::MatchContains);
        driver->setCurrentIndex( (idx<0)? 0:idx );
        idx = wrPolicy->findText(
                    _driver.attribute("wrpolicy"), Qt::MatchContains);
        wrPolicy->setCurrentIndex( (idx<0)? 0:idx );
        idx = format->findText(
                    _driver.attribute("format"), Qt::MatchContains);
        format->setCurrentIndex( (idx<0)? 0:idx );
    } else if ( _driver.isNull() )
        driver->setCurrentIndex(0);
    source->setText(_source.attribute("file"));
    target->setText(_target.attribute("dir"));
    readOnly->setChecked( !_readOnly.isNull() );
}

/* private slots */
void FileFsType::getSourcePath()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                "Source Path",
                "~");
    if ( !fileName.isEmpty() ) source->setText(fileName);
}
