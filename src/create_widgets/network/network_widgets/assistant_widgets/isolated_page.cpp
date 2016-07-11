#include "isolated_page.h"
#include <QVariant>

IsolatedPage::IsolatedPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle("");
    setSubTitle(
    "");

    lt = new QVBoxLayout(this);
    setLayout(lt);
}
int IsolatedPage::nextId() const
{
    return -1;
}
bool IsolatedPage::isComplete() const
{
    return true;
}
