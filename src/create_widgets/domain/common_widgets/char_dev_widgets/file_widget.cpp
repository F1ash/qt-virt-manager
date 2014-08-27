#include "file_widget.h"

FileWidget::FileWidget(QWidget *parent, QString _tag) :
    _QWidget(parent), tag(_tag)
{
    pathLabel = new QLabel("Path:", this);
    path = new QLineEdit(this);

    fileLayout = new QGridLayout(this);
    fileLayout->addWidget(pathLabel, 0, 0);
    fileLayout->addWidget(path, 0, 1);
    setLayout(fileLayout);
}

/* public slots */
QDomDocument FileWidget::getDevDocument() const
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

    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    //qDebug()<<doc.toString();
    return doc;
}
