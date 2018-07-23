#ifndef INTRODUCE_PAGE_H
#define INTRODUCE_PAGE_H

#include <QWizardPage>
#include <QLineEdit>
#include <QRadioButton>
#include <QVBoxLayout>
#include "create_widgets/network/create_virt_network_ass.h"

class IntroducePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit IntroducePage(QWidget *parent = Q_NULLPTR);
    int             nextId() const;
    bool            isComplete() const;

private:
    QLineEdit      *networkName;
    QRadioButton   *natNet, *bridgeNet,
                   *routedNet, *isolatedNet;
    QVBoxLayout    *lt;
};

#endif // INTRODUCE_PAGE_H
