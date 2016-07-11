#include "nated_page.h"
#include <QVariant>

NATedPage::NATedPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle("");
    setSubTitle(
    "");

    lt = new QVBoxLayout(this);
    setLayout(lt);
}
int NATedPage::nextId() const
{
    return -1;
}
bool NATedPage::isComplete() const
{
    return true;
}
