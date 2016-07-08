#ifndef CREATE_VIRT_NETWORK_ASS_H
#define CREATE_VIRT_NETWORK_ASS_H

#include <QWizard>
#include "network_widgets/assistant_widgets/introduce_page.h"

class CreateVirtNetwork_Ass : public QWizard
{
    Q_OBJECT
public:
    explicit CreateVirtNetwork_Ass(QWidget *parent = nullptr);

private:
    IntroducePage      *introPage;
};

#endif // CREATE_VIRT_NETWORK_ASS_H
