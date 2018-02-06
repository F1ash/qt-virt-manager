#include "conclusion_page.h"
#include <QVariant>

ConclusionPage::ConclusionPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Exit from VirtNetwork Assistant"));
    setSubTitle(tr("Done."));
    info = new QLabel(this);
    lt = new QVBoxLayout(this);
    lt->addWidget(info);
    setLayout(lt);
}
void ConclusionPage::initializePage()
{
    QString netName = wizard()->field("NetworkName").toString();
    QString msg = QString(
tr("Build up the data for < %1 > network done.\n\
Other parameters of network you can change later in Network Editor."))
            .arg(netName);
    info->setText(msg);
}
