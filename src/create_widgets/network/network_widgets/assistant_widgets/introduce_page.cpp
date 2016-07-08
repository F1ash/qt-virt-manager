#include "introduce_page.h"

IntroducePage::IntroducePage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle("Welcome to VirtNetwork Assistant");
    setSubTitle(
    "Please choose network type to create it");

    bridgeNet = new QRadioButton(
    "Create network by bridge on host device", this);
    natNet = new QRadioButton(
    "Create network with NAT", this);
    routeNet = new QRadioButton(
    "Create routed network", this);
    isolatedNet = new QRadioButton(
    "Create isolated network for guest-to-guest connectivity", this);
    lt = new QVBoxLayout();
    lt->addWidget(bridgeNet);
    lt->addWidget(natNet);
    lt->addWidget(routeNet);
    lt->addWidget(isolatedNet);
    setLayout(lt);
}
