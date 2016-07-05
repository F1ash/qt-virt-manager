#include "udp_widget.h"

UdpWidget::UdpWidget(QWidget *parent, QString _tag) :
    _QWidget(parent), tag(_tag)
{
    hostLabel = new QLabel("Host:", this);
    host = new QLineEdit(this);
    host->setText("127.0.0.1");
    port1Label = new QLabel("Port:", this);
    port1 = new QSpinBox(this);
    port1->setRange(0, 65535);
    port1->setValue(4555);
    bindHostLabel = new QLabel("Bind Host:", this);
    bindHost = new QLineEdit(this);
    bindHost->setText("127.0.0.1");
    port2Label = new QLabel("Port:", this);
    port2 = new QSpinBox(this);
    port2->setRange(0, 65535);
    port2->setValue(4555);
    udpLayout = new QGridLayout();
    udpLayout->addWidget(hostLabel, 0, 0);
    udpLayout->addWidget(host, 0, 1);
    udpLayout->addWidget(port1Label, 0, 2);
    udpLayout->addWidget(port1, 0, 3);
    udpLayout->addWidget(bindHostLabel, 1, 0);
    udpLayout->addWidget(bindHost, 1, 1);
    udpLayout->addWidget(port2Label, 1, 2);
    udpLayout->addWidget(port2, 1, 3);
    setLayout(udpLayout);
    connect(host, SIGNAL(textChanged(QString)),
            this, SLOT(stateChanged()));
    connect(bindHost, SIGNAL(textChanged(QString)),
            this, SLOT(stateChanged()));
    connect(port1, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(port2, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument UdpWidget::getDataDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source1, _source2, _target, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement(tag);
    _source1 = doc.createElement("source");
    _source1.setAttribute("mode", "bind");
    _source1.setAttribute("host", bindHost->text());
    _source1.setAttribute("service", port2->value());
    _devDesc.appendChild(_source1);

    _source2 = doc.createElement("source");
    _source2.setAttribute("mode", "connect");
    _source2.setAttribute("host", host->text());
    _source2.setAttribute("service", port1->value());
    _devDesc.appendChild(_source2);

    _target = doc.createElement("target");
    _target.setAttribute("port", "0");
    _devDesc.appendChild(_target);

    _devDesc.setAttribute("type", "udp");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    //qDebug()<<doc.toString();
    return doc;
}
void UdpWidget::setDataDescription(const QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _source1, _source2;
    _device = doc
            .firstChildElement("device")
            .firstChildElement(tag);
    _source1 = _device.firstChildElement("source");
    _source2 = _source1.nextSiblingElement("source");
    QString _mode;
    if ( !_source1.isNull() ) {
        _mode = _source1.attribute("mode");
        if ( _mode=="bind" ) {
            bindHost->setText(_source1.attribute("host"));
            port2->setValue(_source1.attribute("service").toInt());
        } else {
            host->setText(_source1.attribute("host"));
            port1->setValue(_source1.attribute("service").toInt());
        };
    };
    if ( !_source2.isNull() ) {
        _mode = _source2.attribute("mode");
        if ( _mode=="bind" ) {
            bindHost->setText(_source2.attribute("host"));
            port2->setValue(_source2.attribute("service").toInt());
        } else {
            host->setText(_source2.attribute("host"));
            port1->setValue(_source2.attribute("service").toInt());
        };
    };
}
