#include "forward_page.h"

ForwardPage::ForwardPage(QWidget *parent) :
    QWizardPage(parent)
{
    setSubTitle(
    ".");

    fr = new Forward_Widget(this);
    lt = new QVBoxLayout(this);
    lt->addWidget(fr);
    setLayout(lt);
    connect(fr, SIGNAL(toggled(bool)),
            this, SIGNAL(completeChanged()));
    connect(fr, SIGNAL(dataChanged()),
            this, SIGNAL(completeChanged()));
}
int ForwardPage::nextId() const
{
    return CreateVirtNetwork_Ass::Page_Conclusion;
}
bool ForwardPage::isComplete() const
{
    return ( fr->isUsed() );
}
QDomDocument ForwardPage::getDataDocument() const
{
    return fr->getDataDocument();
}
