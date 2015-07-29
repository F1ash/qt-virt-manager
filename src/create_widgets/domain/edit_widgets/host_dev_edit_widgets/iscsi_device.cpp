#include "iscsi_device.h"

ISCSI_Device::ISCSI_Device(QWidget *parent, virConnectPtr _conn) :
    QWidget(parent)
{
    hostLabel = new QLabel("Host:", this);
    portLabel = new QLabel("Port:", this);
    host = new QLineEdit(this);
    port = new QSpinBox(this);
    port->setRange(0, 65536);
    hostLayout = new QHBoxLayout(this);
    hostLayout->addWidget(hostLabel, 1);
    hostLayout->addWidget(host, 5);
    hostLayout->addWidget(portLabel, 1);
    hostLayout->addWidget(port, 3);
    hostWdg = new QWidget(this);
    hostWdg->setLayout(hostLayout);
    auth = new _Storage_Auth(this, _conn);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(hostWdg);
    commonLayout->addWidget(auth);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(host, SIGNAL(textEdited(QString)),
            this, SIGNAL(dataChanged()));
    connect(port, SIGNAL(valueChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(auth, SIGNAL(dataChanged()),
            this, SIGNAL(dataChanged()));
}
