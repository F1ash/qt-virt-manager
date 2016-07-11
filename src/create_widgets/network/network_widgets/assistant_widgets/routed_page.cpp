#include "routed_page.h"
#include <QVariant>

RoutedPage::RoutedPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle("");
    setSubTitle(
    "");

    lt = new QVBoxLayout(this);
    setLayout(lt);
}
int RoutedPage::nextId() const
{
    return -1;
}
bool RoutedPage::isComplete() const
{
    return true;
}
