#include "dev_widget.h"

DevWidget::DevWidget(QWidget *parent, QString _tag) :
    _QWidget(parent), tag(_tag)
{
    pathLabel = new QLabel("Path:", this);
    path = new QLineEdit(this);
    devLayout = new QGridLayout(this);
    devLayout->addWidget(pathLabel, 0, 0);
    devLayout->addWidget(path, 0, 1);
    setLayout(devLayout);
}

/* public slots */
QDomDocument DevWidget::getDevDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _target, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement(tag);
    _source = doc.createElement("source");
    _source.setAttribute("path", path->text());
    _devDesc.appendChild(_source);

    _target = doc.createElement("target");
    _target.setAttribute("port", 0);
    _devDesc.appendChild(_target);

    _devDesc.setAttribute("type", "dev");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    //qDebug()<<doc.toString();
    return doc;
}
