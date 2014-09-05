#include "mac_address.h"

MAC_Address::MAC_Address(QWidget *parent) :
    QWidget(parent)
{
    useMac = new QCheckBox("MAC:", this);
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
}

/* public slots */
QString MAC_Address::getMACAddress() const
{
    if ( mac->isEnabled() )
        return mac->text();
    else return QString();
}
