#include "tcp_widget.h"

TcpWidget::TcpWidget(QWidget *parent, QString _tag) :
    _QWidget(parent), tag(_tag)
{
    modeLabel = new QLabel("Mode:", this);
    mode = new QComboBox(this);
    mode->addItem("Server mode (bind)", "bind");
    mode->addItem("Client mode (connect)", "connect");
    hostLabel = new QLabel("Host:", this);
    host = new QLineEdit(this);
    host->setText("127.0.0.1");
    portLabel = new QLabel("Port:", this);
    port = new QSpinBox(this);
    port->setRange(0, 65535);
    port->setValue(4555);
    telnetLabel = new QLabel("Use Telnet:", this);
    telnet = new QComboBox(this);
    telnet->addItem("Usual", "raw");
    telnet->addItem("Telnet", "telnet");
    telnet->addItem("Secure Telnet", "telnets");
    telnet->addItem("TLS", "tls");
    tcpLayout = new QGridLayout();
    tcpLayout->addWidget(modeLabel, 0, 0);
    tcpLayout->addWidget(mode, 0, 1);
    tcpLayout->addWidget(hostLabel, 1, 0);
    tcpLayout->addWidget(host, 1, 1);
    tcpLayout->addWidget(portLabel, 1, 2);
    tcpLayout->addWidget(port, 1, 3);
    tcpLayout->addWidget(telnetLabel, 3, 0);
    tcpLayout->addWidget(telnet, 3, 1);
    setLayout(tcpLayout);
    connect(mode, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(host, SIGNAL(textChanged(QString)),
            this, SLOT(stateChanged()));
    connect(port, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(telnet, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument TcpWidget::getDataDocument() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _protocol, _target, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement(tag);
    _source = doc.createElement("source");
    _source.setAttribute(
                "mode",
                mode->itemData(
                    mode->currentIndex(),
                    Qt::UserRole).toString());
    _source.setAttribute("host", host->text());
    _source.setAttribute("service", port->value());
    _devDesc.appendChild(_source);

    _protocol = doc.createElement("protocol");
    _protocol.setAttribute(
                "type",
                telnet->itemData(
                    telnet->currentIndex(),
                    Qt::UserRole).toString());
    _devDesc.appendChild(_protocol);

    _target = doc.createElement("target");
    _target.setAttribute("port", "0");
    _devDesc.appendChild(_target);

    _devDesc.setAttribute("type", "tcp");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    //qDebug()<<doc.toString();
    return doc;
}
void TcpWidget::setDataDescription(QString &xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(xmlDesc);
    QDomElement _device, _source, _protocol, _target;
    _device = doc
            .firstChildElement("device")
            .firstChildElement(tag);
    int idx;
    _source = _device.firstChildElement("source");
    if ( !_source.isNull() ) {
        idx = mode->findData(
                    _source.attribute("mode"),
                    Qt::UserRole,
                    Qt::MatchContains);
        mode->setCurrentIndex( (idx<0)? 0:idx );
        host->setText(_source.attribute("host"));
        port->setValue(_source.attribute("service").toInt());
    };
    _protocol = _device.firstChildElement("protocol");
    if ( !_protocol.isNull() ) {
        idx = telnet->findData(
                    _protocol.attribute("type"),
                    Qt::UserRole,
                    Qt::MatchContains);
        telnet->setCurrentIndex( (idx<0)? 0:idx );
    };
    _target = _device.firstChildElement("target");
    if ( !_target.isNull() ) {

    };
}
