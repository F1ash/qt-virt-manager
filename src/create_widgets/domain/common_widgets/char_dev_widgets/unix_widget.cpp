#include "unix_widget.h"

UnixWidget::UnixWidget(QWidget *parent) :
    _QWidget(parent)
{
    pathLabel = new QLabel("Path:", this);
    path = new QLineEdit(this);
    modeLabel = new QLabel("Mode:", this);
    mode = new QComboBox(this);
    mode->addItem("Server mode (bind)", "bind");
    mode->addItem("Client mode (connect)", "connect");
    unixLayout = new QGridLayout(this);
    unixLayout->addWidget(pathLabel, 0, 0);
    unixLayout->addWidget(path, 0, 1);
    unixLayout->addWidget(modeLabel, 1, 0);
    unixLayout->addWidget(mode, 1, 1);
    setLayout(unixLayout);
}
UnixWidget::~UnixWidget()
{
    delete pathLabel;
    pathLabel = 0;
    delete path;
    path = 0;
    delete unixLayout;
    unixLayout = 0;
}

/* public slots */
QDomNodeList UnixWidget::getNodeList() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _target;
    _source = doc.createElement("source");
    _source.setAttribute("path", path->text());
    _source.setAttribute("mode", mode->itemData(mode->currentIndex(), Qt::UserRole).toString());
    doc.appendChild(_source);

    _target = doc.createElement("target");
    _target.setAttribute("port", 0);
    doc.appendChild(_target);

    //qDebug()<<doc.toString();
    return doc.childNodes();
}
void UnixWidget::setPath(QString text)
{
    path->setText(text);
}
