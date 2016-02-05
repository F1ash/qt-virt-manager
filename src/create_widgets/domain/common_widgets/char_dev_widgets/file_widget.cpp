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
    connect(path, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument FileWidget::getDataDocument() const
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

    _devDesc.setAttribute("type", "file");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    //qDebug()<<doc.toString();
    return doc;
}
void FileWidget::setDataDescription(QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _source, _target;
    _device = doc
            .firstChildElement("device")
            .firstChildElement(tag);
    _source = _device.firstChildElement("source");
    if ( !_source.isNull() ) {
        path->setText(_source.attribute("path"));
    };
    _target = _device.firstChildElement("target");
    if ( !_target.isNull() ) {
    };
}
