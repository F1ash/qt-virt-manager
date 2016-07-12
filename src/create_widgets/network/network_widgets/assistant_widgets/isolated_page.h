#ifndef ISOLATED_PAGE_H
#define ISOLATED_PAGE_H

#include <QWizardPage>
#include <QTabWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include "create_widgets/network/create_virt_network_ass.h"
#include "create_widgets/network/network_widgets/addressing_widgets/ip_widgets/_ipv4.h"
#include "create_widgets/network/network_widgets/addressing_widgets/ip_widgets/_ipv6.h"

class IsolatedPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit IsolatedPage(QWidget *parent = nullptr);
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

#endif // ISOLATED_PAGE_H
