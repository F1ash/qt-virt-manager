#ifndef ROUTED_PAGE_H
#define ROUTED_PAGE_H

#include <QWizardPage>
#include <QVBoxLayout>
#include "create_widgets/network/create_virt_network_ass.h"

class RoutedPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit RoutedPage(QWidget *parent = nullptr);
    int             nextId() const;
    bool            isComplete() const;

private:
    QVBoxLayout    *lt;
};

#endif // ROUTED_PAGE_H
