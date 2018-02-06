#include "no_gateway_page.h"

NoGatewayPage::NoGatewayPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Isolated network"));
    setSubTitle(
tr("This virtual network can be used\
 for guest-to-guest communication only."));

    ipv6 = new QCheckBox(tr("Use IPv6 network"), this);
    lt = new QVBoxLayout(this);
    lt->addWidget(ipv6);
    setLayout(lt);
    registerField("NoGatewayIPv6", ipv6);
}
int NoGatewayPage::nextId() const
{
    return CreateVirtNetwork_Ass::Page_Conclusion;
}
