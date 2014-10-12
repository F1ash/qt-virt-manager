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
    delete sourceLabel;
    sourceLabel = NULL;
    commonLayout->addWidget(new QLabel("Source:", this), 4, 0);
    source->setPlaceholderText("Source guest directory");
    target->setPlaceholderText("Target guest directory");
}

/* public slots */
QDomDocument BindFsType::getDataDocument() const
{
    QDomDocument doc = QDomDocument();
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
