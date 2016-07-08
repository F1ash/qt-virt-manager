#ifndef INTRODUCE_PAGE_H
#define INTRODUCE_PAGE_H

#include <QWizardPage>
#include <QRadioButton>
#include <QVBoxLayout>

class IntroducePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit IntroducePage(QWidget *parent = nullptr);

private:
    QRadioButton   *natNet, *bridgeNet,
                   *routeNet, *isolatedNet;
    QVBoxLayout    *lt;
};

#endif // INTRODUCE_PAGE_H
