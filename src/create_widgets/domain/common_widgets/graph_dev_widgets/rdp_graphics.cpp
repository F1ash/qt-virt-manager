#include "rdp_graphics.h"

RDP_Graphics::RDP_Graphics(QWidget *parent) :
    _QWidget(parent)
{
    autoPort = new QCheckBox("AutoPort", this);
    port = new QSpinBox(this);
    port->setRange(10, 65535);
    replaceUser = new QCheckBox("Replace\nUser", this);
    multiUser = new QCheckBox("MultiUser", this);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(autoPort, 0, 0, Qt::AlignTop);
    commonLayout->addWidget(port, 0, 1, Qt::AlignTop);
    commonLayout->addWidget(replaceUser, 1, 0, Qt::AlignTop);
    commonLayout->addWidget(multiUser, 1, 1, Qt::AlignTop);
    setLayout(commonLayout);
    connect(autoPort, SIGNAL(toggled(bool)),
            this, SLOT(autoPortChanged(bool)));
    autoPort->setChecked(true);
}

/* public slots */
QDomDocument RDP_Graphics::getDevDocument() const
{
    return QDomDocument();
}

/* private slots */
void RDP_Graphics::autoPortChanged(bool state)
{
    port->setVisible(!state);
}
