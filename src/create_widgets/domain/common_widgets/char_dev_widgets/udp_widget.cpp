#include "udp_widget.h"

UdpWidget::UdpWidget(QWidget *parent) :
    _QWidget(parent)
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
}
UdpWidget::~UdpWidget()
{
    delete hostLabel;
    hostLabel = 0;
    delete host;
    host = 0;
    delete port1Label;
    port1Label = 0;
    delete port1;
    port1 = 0;
    delete bindHostLabel;
    bindHostLabel = 0;
    delete bindHost;
    bindHost = 0;
    delete port2Label;
    port2Label = 0;
    delete port2;
    port2 = 0;
    delete udpLayout;
    udpLayout = 0;
}

/* public slots */
QDomNodeList UdpWidget::getNodeList() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source1, _source2, _target;
    _source1 = doc.createElement("source");
    _source1.setAttribute("mode", "bind");
    _source1.setAttribute("host", bindHost->text());
    _source1.setAttribute("service", port2->value());
    doc.appendChild(_source1);

    _source2 = doc.createElement("source");
    _source2.setAttribute("mode", "connect");
    _source2.setAttribute("host", host->text());
    _source2.setAttribute("service", port1->value());
    doc.appendChild(_source2);

    _target = doc.createElement("target");
    _target.setAttribute("port", "0");
    doc.appendChild(_target);

    //qDebug()<<doc.toString();
    return doc.childNodes();
}
