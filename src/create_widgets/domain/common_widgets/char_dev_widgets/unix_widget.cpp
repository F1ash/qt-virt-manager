#include "unix_widget.h"

UnixWidget::UnixWidget(QWidget *parent, QString _tag) :
    _QWidget(parent), tag(_tag)
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

/* public slots */
QDomDocument UnixWidget::getDevDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _target, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement(tag);
    _source = doc.createElement("source");
    _source.setAttribute("path", path->text());
    _source.setAttribute("mode", mode->itemData(mode->currentIndex(), Qt::UserRole).toString());
    _devDesc.appendChild(_source);

    _target = doc.createElement("target");
    _target.setAttribute("port", 0);
    _devDesc.appendChild(_target);

    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    //qDebug()<<doc.toString();
    return doc;
}
void UnixWidget::setPath(QString text)
{
    path->setText(text);
}
