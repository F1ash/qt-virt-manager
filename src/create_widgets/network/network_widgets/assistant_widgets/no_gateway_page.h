#ifndef NO_GATEWAY_PAGE_H
#define NO_GATEWAY_PAGE_H

#include <QWizardPage>
#include <QCheckBox>
#include <QVBoxLayout>
#include "create_widgets/network/create_virt_network_ass.h"

class NoGatewayPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit NoGatewayPage(QWidget *parent = Q_NULLPTR);
    int             nextId() const;

private:
    QCheckBox      *ipv6;
    QVBoxLayout    *lt;
};

#endif // NO_GATEWAY_PAGE_H
