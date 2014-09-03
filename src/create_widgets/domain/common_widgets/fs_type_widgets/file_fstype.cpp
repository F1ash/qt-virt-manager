#include "file_fstype.h"

FileFsType::FileFsType(QWidget *parent, QString _type) :
    _FsType(parent, _type)
{
    source->setPlaceholderText("Source host file");
    target->setPlaceholderText("Target guest directory");
    connect(sourceLabel, SIGNAL(clicked()),
            this, SLOT(getSourcePath()));
}

/* public slots */
QDomDocument FileFsType::getDevDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _target, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("filesystem");
    if ( connType=="qemu" && accessMode->currentText()!="default" ) {
        _devDesc.setAttribute("accessmode", accessMode->currentText());
    };
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
    }
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

/* private slots */
void FileFsType::getSourcePath()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                "Source Path",
                "~");
    if ( !fileName.isEmpty() ) source->setText(fileName);
}
void FileFsType::driverTypeChanged(QString _type)
{
    format->clear();
    /*
     * LXC supports a type of "loop", with a format of "raw" or "nbd" with any format.
     * QEMU supports a type of "path" or "handle", but no formats.
     */
    formatLabel->setVisible( connType=="lxc" && _type!="default" );
    format->setVisible( connType=="lxc" && _type!="default" );
    if ( _type=="loop" ) {
        format->addItem("raw");
    } else if ( _type=="nbd" ) {
        format->addItems(FORMAT_TYPES);
    }
}
