#include "iscsi_device.h"

ISCSI_Device::ISCSI_Device(QWidget *parent, virConnectPtr *connPtrPtr) :
    QWidget(parent)
{
    hostLabel = new QLabel(tr("Host:"), this);
    portLabel = new QLabel(tr("Port:"), this);
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
    auth = new _Storage_Auth(this, connPtrPtr);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(hostWdg);
    commonLayout->addWidget(auth);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(host, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(port, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(auth, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
void ISCSI_Device::stateChanged()
{
    emit dataChanged();
}
