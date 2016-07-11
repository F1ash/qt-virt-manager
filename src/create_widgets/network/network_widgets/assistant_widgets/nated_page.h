#ifndef NATED_PAGE_H
#define NATED_PAGE_H

#include <QWizardPage>
#include <QVBoxLayout>
#include "create_widgets/network/create_virt_network_ass.h"

class NATedPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit NATedPage(QWidget *parent = nullptr);
    int             nextId() const;
    bool            isComplete() const;

private:
    QVBoxLayout    *lt;
};

#endif // NATED_PAGE_H
