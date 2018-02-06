#include "introduce_page.h"
#include <QVariant>

IntroducePage::IntroducePage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Welcome to VirtNetwork Assistant"));
    setSubTitle(
    tr("Please choose network type to create it."));

    networkName = new QLineEdit(this);
    networkName->setPlaceholderText(
                tr("Set new Network name"));
    bridgeNet = new QRadioButton(
                tr("Create network using existing host bridge"),
                this);
    natNet = new QRadioButton(
                tr("Create NAT based network"), this);
    routedNet = new QRadioButton(
                tr("Create routed network"), this);
    isolatedNet = new QRadioButton(
                tr("Create isolated network"), this);
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
        return CreateVirtNetwork_Ass::Page_Forward;
    } else if ( routedNet->isChecked() ) {
        return CreateVirtNetwork_Ass::Page_Forward;
    } else if ( isolatedNet->isChecked() ) {
        return CreateVirtNetwork_Ass::Page_IP;
    };
    return CreateVirtNetwork_Ass::Page_Conclusion;
}
bool IntroducePage::isComplete() const
{
    return (    !networkName->text().isEmpty() &&
                ( bridgeNet->isChecked()    ||
                natNet->isChecked()         ||
                routedNet->isChecked()      ||
                isolatedNet->isChecked() )      );
}
