#ifndef IP_PAGE_H
#define IP_PAGE_H

#include <QWizardPage>
#include <QTabWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include "create_widgets/network/create_virt_network_ass.h"
#include "create_widgets/network/network_widgets/addressing_widgets/ip_widgets/_ipv4.h"
#include "create_widgets/network/network_widgets/addressing_widgets/ip_widgets/_ipv6.h"

class IP_Page : public QWizardPage
{
    Q_OBJECT
public:
    explicit IP_Page(QWidget *parent = Q_NULLPTR);
    void            initializePage();
    int             nextId() const;
    bool            isComplete() const;
    bool            isUsed() const;
    QDomDocument    getDataDocument() const;

private:
    _IPv4          *ipv4;
    _IPv6          *ipv6;
    QTabWidget     *tabs;
    QScrollArea    *scrolled;
    QVBoxLayout    *lt;
};

#endif // IP_PAGE_H
