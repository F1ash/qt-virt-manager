#ifndef FORWARD_PAGE_H
#define FORWARD_PAGE_H

#include <QWizardPage>
#include <QVBoxLayout>
#include <QDomDocument>
#include "create_widgets/network/create_virt_network_ass.h"
#include "create_widgets/network/network_widgets/forward_widget.h"

class ForwardPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ForwardPage(QWidget *parent = Q_NULLPTR);
    Forward_Widget *fr;
    void            initializePage();
    int             nextId() const;
    bool            isComplete() const;
    QDomDocument    getDataDocument() const;

private:
    QVBoxLayout    *lt;
};

#endif // FORWARD_PAGE_H
