#ifndef BRIDGE_PAGE_H
#define BRIDGE_PAGE_H

#include <QWizardPage>
#include <QVBoxLayout>
#include <QDomDocument>
#include "create_widgets/network/create_virt_network_ass.h"
#include "create_widgets/network/network_widgets/bridge_widget.h"

class BridgePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit BridgePage(QWidget *parent = nullptr);
    Bridge_Widget  *br;
    void            initializePage();
    int             nextId() const;
    bool            isComplete() const;
    QDomDocument    getDataDocument() const;

private:
    QVBoxLayout    *lt;
};

#endif // BRIDGE_PAGE_H
