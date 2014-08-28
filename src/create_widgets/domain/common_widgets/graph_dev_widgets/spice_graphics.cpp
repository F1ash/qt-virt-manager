#include "spice_graphics.h"

#define KEYMAPs QStringList()<<"auto"<<"en-gb"<<"en-us"<<"ru"<<"fr"<<"de"<<"is"<<"it"<<"ja"

Spice_Graphics::Spice_Graphics(QWidget *parent) :
    _QWidget(parent)
{
    addrLabel = new QLabel("Address:", this);
    address = new QComboBox(this);
    address->setEditable(false);
    address->addItem("HyperVisor default", "default");
    address->addItem("LocalHost only", "local");
    address->addItem("All Interfaces", "all");
    autoPort = new QCheckBox("AutoPort", this);
    port = new QSpinBox(this);
    port->setRange(10, 65535);
    port->setValue(5900);
    port->setEnabled(false);
    usePassw = new QCheckBox("Password", this);
    passw = new QLineEdit(this);
    passw->setEnabled(false);
    keymapLabel = new QLabel("Keymap:", this);
    keymap = new QComboBox(this);
    keymap->setEditable(true);
    keymap->addItems(KEYMAPs);
    keymap->setEnabled(false);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(addrLabel, 0, 0);
    commonLayout->addWidget(address, 0, 1);
    commonLayout->addWidget(autoPort, 1, 0);
    commonLayout->addWidget(port, 1, 1);
    commonLayout->addWidget(usePassw, 2, 0);
    commonLayout->addWidget(passw, 2, 1);
    commonLayout->addWidget(keymapLabel, 3, 0, Qt::AlignTop);
    commonLayout->addWidget(keymap, 3, 1, Qt::AlignTop);
    setLayout(commonLayout);
    connect(address, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(addressEdit(QString)));
    connect(autoPort, SIGNAL(toggled(bool)),
            this, SLOT(usePort(bool)));
    connect(usePassw, SIGNAL(toggled(bool)),
            this, SLOT(usePassword(bool)));
    autoPort->setChecked(true);
    usePassw->setChecked(false);
}

/* public slots */
QDomDocument Spice_Graphics::getDevDocument() const
{
    QDomDocument doc;
    QDomElement _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("graphics");
    _devDesc.setAttribute("type", "spice");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}

/* private slots */
void Spice_Graphics::usePort(bool state)
{
    port->setEnabled(!state);
}
void Spice_Graphics::usePassword(bool state)
{
    passw->setEnabled(state);
    keymap->setEnabled(state);
}
void Spice_Graphics::addressEdit(QString s)
{
    if ( s == "Custom" ) {
        address->setEditable(true);
        address->setEditText("");
    } else {
        address->setEditable(false);
    }
}
