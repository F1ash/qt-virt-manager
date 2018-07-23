#ifndef CONCLUSION_PAGE_H
#define CONCLUSION_PAGE_H

#include <QWizardPage>
#include <QLabel>
#include <QVBoxLayout>

class ConclusionPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ConclusionPage(QWidget *parent = Q_NULLPTR);
    void            initializePage();

private:
    QLabel         *info;
    QVBoxLayout    *lt;
};

#endif // CONCLUSION_PAGE_H
