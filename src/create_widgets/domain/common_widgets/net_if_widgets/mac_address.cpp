#include "mac_address.h"

MAC_Address::MAC_Address(QWidget *parent) :
    _Changed(parent)
{
    useMac = new QCheckBox(tr("MAC:"), this);
    useMac->setLayoutDirection(Qt::RightToLeft);
    mac = new QLineEdit(this);
    mac->setPlaceholderText("11:22:33:44:55:66");
    mac->setEnabled(false);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(useMac, 0, 0);
    commonLayout->addWidget(mac, 0, 1);
    setLayout(commonLayout);
    connect(useMac, SIGNAL(toggled(bool)),
            mac, SLOT(setEnabled(bool)));
    // dataChanged connects
    connect(useMac, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(mac, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
}

/* public slots */
bool MAC_Address::isUsed() const
{
    return useMac->isChecked();
}
void MAC_Address::setUsage(bool state)
{
    useMac->setChecked(state);
}
QString MAC_Address::getMACAddress() const
{
    if ( mac->isEnabled() )
        return mac->text();
    else return QString();
}
void MAC_Address::setMACAddress(QString &_addr)
{
    mac->setText(_addr);
}
