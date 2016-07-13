#include "forward_page.h"

ForwardPage::ForwardPage(QWidget *parent) :
    QWizardPage(parent)
{
    fr = new Forward_Widget(this);
    lt = new QVBoxLayout(this);
    lt->addWidget(fr);
    setLayout(lt);
    connect(fr, SIGNAL(toggled(bool)),
            this, SIGNAL(completeChanged()));
    connect(fr, SIGNAL(dataChanged()),
            this, SIGNAL(completeChanged()));
}
void ForwardPage::initializePage()
{
    QString t, s;
    if ( wizard()->field("NATedType").toBool() ) {
        fr->setCurrentMode("nat");
        t = "NAT based network";
        s =
    "The guests will be directly connected to the virtual network.";
    } else if ( wizard()->field("RoutedType").toBool() ) {
        fr->setCurrentMode("route");
        t = "Routed Network";
        s =
    "The guests will be directly connected to the virtual network.";
    };
    setTitle(t);
    setSubTitle(s);
}
int ForwardPage::nextId() const
{
    return CreateVirtNetwork_Ass::Page_IP;
}
bool ForwardPage::isComplete() const
{
    return ( fr->isUsed() );
}
QDomDocument ForwardPage::getDataDocument() const
{
    return fr->getDataDocument();
}
