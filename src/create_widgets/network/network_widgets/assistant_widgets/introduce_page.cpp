#include "introduce_page.h"
#include <QVariant>

IntroducePage::IntroducePage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle("Welcome to VirtNetwork Assistant");
    setSubTitle(
    "Please choose network type to create it.");

    networkName = new QLineEdit(this);
    networkName->setPlaceholderText("Set new Network name");
    bridgeNet = new QRadioButton(
    "Create network using existing host bridge", this);
    natNet = new QRadioButton(
    "Create NAT based network", this);
    routedNet = new QRadioButton(
    "Create routed network", this);
    isolatedNet = new QRadioButton(
    "Create isolated network", this);
    lt = new QVBoxLayout(this);
    lt->addWidget(networkName);
    lt->addWidget(bridgeNet);
    lt->addWidget(natNet);
    lt->addWidget(routedNet);
    lt->addWidget(isolatedNet);
    setLayout(lt);
    connect(networkName, SIGNAL(textChanged(QString)),
            this, SIGNAL(completeChanged()));
    connect(bridgeNet, SIGNAL(toggled(bool)),
            this, SIGNAL(completeChanged()));
    connect(natNet, SIGNAL(toggled(bool)),
            this, SIGNAL(completeChanged()));
    connect(routedNet, SIGNAL(toggled(bool)),
            this, SIGNAL(completeChanged()));
    connect(isolatedNet, SIGNAL(toggled(bool)),
            this, SIGNAL(completeChanged()));
    registerField("NetworkName", networkName);
    registerField("BridgeType", bridgeNet);
    registerField("NATedType", natNet);
    registerField("RoutedType", routedNet);
    registerField("IsolatedType", isolatedNet);
}
int IntroducePage::nextId() const
{
    if ( bridgeNet->isChecked() ) {
        return CreateVirtNetwork_Ass::Page_Bridge;
    } else if ( natNet->isChecked() ) {
        return CreateVirtNetwork_Ass::Page_NATed;
    } else if ( routedNet->isChecked() ) {
        return CreateVirtNetwork_Ass::Page_Routed;
    } else if ( isolatedNet->isChecked() ) {
        return CreateVirtNetwork_Ass::Page_Isolated;
    }
}
bool IntroducePage::isComplete() const
{
    return (    !networkName->text().isEmpty() &&
                ( bridgeNet->isChecked()    ||
                natNet->isChecked()         ||
                routedNet->isChecked()      ||
                isolatedNet->isChecked() )      );
}
