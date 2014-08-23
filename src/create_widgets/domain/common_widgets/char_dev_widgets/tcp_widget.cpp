#include "tcp_widget.h"

TcpWidget::TcpWidget(QWidget *parent) :
    _QWidget(parent)
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
}
TcpWidget::~TcpWidget()
{
    delete modeLabel;
    modeLabel = 0;
    delete mode;
    mode = 0;
    delete hostLabel;
    hostLabel = 0;
    delete host;
    host = 0;
    delete portLabel;
    portLabel = 0;
    delete port;
    port = 0;
    delete telnetLabel;
    telnetLabel = 0;
    delete telnet;
    telnet = 0;
    delete tcpLayout;
    tcpLayout = 0;
}

/* public slots */
QDomNodeList TcpWidget::getNodeList() const
{
    QDomDocument doc = QDomDocument();
    QDomElement _source, _protocol, _target;
    _source = doc.createElement("source");
    _source.setAttribute("mode", mode->itemData(mode->currentIndex(), Qt::UserRole).toString());
    _source.setAttribute("host", host->text());
    _source.setAttribute("service", port->value());
    doc.appendChild(_source);

    _protocol = doc.createElement("protocol");
    _protocol.setAttribute("type", telnet->itemData(telnet->currentIndex(), Qt::UserRole).toString());
    doc.appendChild(_protocol);

    _target = doc.createElement("target");
    _target.setAttribute("port", "0");
    doc.appendChild(_target);

    //qDebug()<<doc.toString();
    return doc.childNodes();
}
