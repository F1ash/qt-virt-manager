#include "pty_widget.h"

PtyWidget::PtyWidget(QWidget *parent, QString _tag) :
    _QWidget(parent), tag(_tag)
{
    ptyLayout = new QGridLayout();
}

/* public slots */
QDomDocument PtyWidget::getDevDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement(tag);

    _devDesc.setAttribute("type", "pty");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    //qDebug()<<doc.toString();
    return doc;
}
