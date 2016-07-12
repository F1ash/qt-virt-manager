#include "isolated_page.h"
#include <QVariant>

IsolatedPage::IsolatedPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle("Isolated network");
    setSubTitle(
    "The guests can talk to each other (and the host OS\
 if IP&DHCP used), but can't reach any other machines on the LAN");

    ipv4 = new _IPv4(this, false, 4);
    ipv4->withoutGateway(true);
    ipv6 = new _IPv6(this, false, 6);
    ipv6->withoutGateway(true);
    tabs = new QTabWidget(this);
    tabs->addTab(ipv4, "IPv4");
    tabs->addTab(ipv6, "IPv6");
    scrolled = new QScrollArea(this);
    scrolled->setWidgetResizable(true);
    scrolled->setWidget(tabs);
    lt = new QVBoxLayout(this);
    lt->addWidget(scrolled);
    setLayout(lt);
    connect(ipv4, SIGNAL(dataChanged()),
            this, SIGNAL(completeChanged()));
    connect(ipv6, SIGNAL(dataChanged()),
            this, SIGNAL(completeChanged()));
}
int IsolatedPage::nextId() const
{
    if ( ipv4->IP_data_isEmpty() &&
         ipv6->IP_data_isEmpty() ) {
        // no gayeway
        return CreateVirtNetwork_Ass::Page_NoGateway;
    };
    return CreateVirtNetwork_Ass::Page_Conclusion;
}
bool IsolatedPage::isComplete() const
{
    return true;
}
bool IsolatedPage::isUsed() const
{
    return ( !ipv4->IP_data_isEmpty() ||
             !ipv6->IP_data_isEmpty() );
}
QDomDocument IsolatedPage::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _ip4, _ip6;
    if ( !ipv4->IP_data_isEmpty() ) {
        _ip4 = doc.createElement("ip");
        _ip4.setAttribute(
                    "address",
                    ipv4->address->text());
        _ip4.setAttribute(
                    ipv4->significantBitsL->currentText(),
                    ipv4->significantBits->text());
        if ( ipv4->useDHCP->isUsed() ) {
            _ip4.appendChild(
                        ipv4->useDHCP->getDataDocument());
        };
        doc.appendChild(_ip4);
    };
    if ( !ipv6->IP_data_isEmpty() ) {
        _ip6 = doc.createElement("ip");
        _ip6.setAttribute("family", "ipv6");
        _ip6.setAttribute(
                    "address",
                    ipv6->address->text());
        _ip6.setAttribute(
                    ipv6->significantBitsL->currentText(),
                    ipv6->significantBits->text());
        if ( ipv6->useDHCP->isUsed() ) {
            _ip6.appendChild(
                        ipv6->useDHCP->getDataDocument());
        };
        doc.appendChild(_ip6);
    };
    return doc;
}
