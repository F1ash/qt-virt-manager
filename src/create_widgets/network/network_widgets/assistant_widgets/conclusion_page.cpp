#include "conclusion_page.h"
#include <QVariant>

ConclusionPage::ConclusionPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle("Exit from VirtNetwork Assistant");
    setSubTitle("Done.");
    info = new QLabel(this);
    lt = new QVBoxLayout(this);
    lt->addWidget(info);
    setLayout(lt);
}
void ConclusionPage::initializePage()
{
    QString netName = wizard()->field("NetworkName").toString();
    QString msg = QString(
    "Build up the data for < %1 > network done.")
            .arg(netName);
    info->setText(msg);
}
