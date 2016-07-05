#include "bind_fstype.h"

BindFsType::BindFsType(QWidget *parent, QString _type) :
    _FsType(parent, _type)
{
    wrPolicyLabel->setVisible(false);
    wrPolicy->setVisible(false);
    formatLabel->setVisible(false);
    format->setVisible(false);
    driverLabel->setVisible(false);
    driver->setVisible(false);
    commonLayout->removeWidget(sourceLabel);
    sourceLabel->deleteLater();
    commonLayout->addWidget(new QLabel("Source:", this), 4, 0);
    source->setPlaceholderText("Source guest directory");
    target->setPlaceholderText("Target guest directory");
    // dataChanged connections
    connect(source, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(target, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(readOnly, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument BindFsType::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _source, _target, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("filesystem");
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
    _devDesc.setAttribute("type", "bind");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void BindFsType::setDataDescription(const QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _source, _target, _readOnly, _device;
    _device = doc.firstChildElement("device")
            .firstChildElement("filesystem");
    _source = _device.firstChildElement("source");
    _target = _device.firstChildElement("target");
    _readOnly = _device.firstChildElement("readonly");
    source->setText(_source.attribute("dir"));
    target->setText(_target.attribute("dir"));
    readOnly->setChecked( !_readOnly.isNull() );
}
