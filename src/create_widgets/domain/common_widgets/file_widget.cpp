#include "file_widget.h"

FileWidget::FileWidget(QWidget *parent) :
    _QWidget(parent)
{
    pathLabel = new QLabel("Path:", this);
    path = new QLineEdit(this);

    fileLayout = new QGridLayout(this);
    fileLayout->addWidget(pathLabel, 0, 0);
    fileLayout->addWidget(path, 0, 1);
    setLayout(fileLayout);
}
FileWidget::~FileWidget()
{
    delete pathLabel;
    pathLabel = 0;
    delete path;
    path = 0;
    delete fileLayout;
    fileLayout = 0;
}

/* public slots */
QDomNodeList FileWidget::getNodeList() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _target;
    _source = doc.createElement("source");
    _source.setAttribute("path", path->text());
    doc.appendChild(_source);

    _target = doc.createElement("target");
    _target.setAttribute("port", 0);
    doc.appendChild(_target);

    //qDebug()<<doc.toString();
    return doc.childNodes();
}
