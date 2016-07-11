#ifndef ISOLATED_PAGE_H
#define ISOLATED_PAGE_H

#include <QWizardPage>
#include <QVBoxLayout>
#include "create_widgets/network/create_virt_network_ass.h"

class IsolatedPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit IsolatedPage(QWidget *parent = nullptr);
    int             nextId() const;
    bool            isComplete() const;

private:
    QVBoxLayout    *lt;
};

#endif // ISOLATED_PAGE_H
