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
    commonLayout->addWidget(autoPort, 0, 0);
    commonLayout->addWidget(port, 0, 1);
    commonLayout->addWidget(replaceUser, 1, 0, Qt::AlignTop);
    commonLayout->addWidget(multiUser, 1, 1, Qt::AlignTop);
    setLayout(commonLayout);
    connect(autoPort, SIGNAL(toggled(bool)),
            this, SLOT(autoPortChanged(bool)));
    autoPort->setChecked(true);
    // dataChanged connections
    connect(autoPort, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(port, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(replaceUser, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(multiUser, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument RDP_Graphics::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("graphics");
    _devDesc.setAttribute("type", "rdp");
    if ( autoPort->isChecked() ) {
        _devDesc.setAttribute("autoport", "yes");
    } else {
        _devDesc.setAttribute("port", port->text());
    };
    if ( multiUser->isChecked() ) _devDesc.setAttribute("multiUser", "yes");
    if ( replaceUser->isChecked() ) _devDesc.setAttribute("replaceUser", "yes");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void RDP_Graphics::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    QDomElement _device;
    doc.setContent(_xmlDesc);
    _device = doc.firstChildElement("device")
            .firstChildElement("graphics");
    autoPort->setChecked(
                _device.attribute("autoport")=="yes");
    port->setValue(_device.attribute("port", "10").toInt());
    multiUser->setChecked(
                _device.attribute("multiUser")=="yes");
    replaceUser->setChecked(
                _device.attribute("replaceUser")=="yes");
}

/* private slots */
void RDP_Graphics::autoPortChanged(bool state)
{
    port->setVisible(!state);
}
