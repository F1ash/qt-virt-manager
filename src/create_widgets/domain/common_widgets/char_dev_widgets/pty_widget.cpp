#include "pty_widget.h"

PtyWidget::PtyWidget(QWidget *parent, QString _tag) :
    _QWidget(parent), tag(_tag)
{
    ptyLayout = new QGridLayout();
}

/* public slots */
QDomDocument PtyWidget::getDataDocument() const
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
void PtyWidget::setDataDescription(const QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _device, _source, _target;
    _device = doc
            .firstChildElement("device")
            .firstChildElement(tag);
    _source = _device.firstChildElement("source");
    if ( !_source.isNull() ) {
    };
    _target = _device.firstChildElement("target");
    if ( !_target.isNull() ) {
    };
}
