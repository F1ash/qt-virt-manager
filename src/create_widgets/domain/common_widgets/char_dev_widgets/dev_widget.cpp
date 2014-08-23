#include "dev_widget.h"

DevWidget::DevWidget(QWidget *parent) :
    _QWidget(parent)
{
    pathLabel = new QLabel("Path:", this);
    path = new QLineEdit(this);
    devLayout = new QGridLayout(this);
    devLayout->addWidget(pathLabel, 0, 0);
    devLayout->addWidget(path, 0, 1);
    setLayout(devLayout);
}
DevWidget::~DevWidget()
{
    delete pathLabel;
    pathLabel = 0;
    delete path;
    path = 0;
    delete devLayout;
    devLayout = 0;
}

/* public slots */
QDomNodeList DevWidget::getNodeList() const
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
