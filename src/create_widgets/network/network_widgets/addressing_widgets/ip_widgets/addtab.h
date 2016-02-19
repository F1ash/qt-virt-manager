#ifndef ADDTAB_H
#define ADDTAB_H

#include <QMenu>

class AddTab : public QMenu
{
    Q_OBJECT
public:
    explicit AddTab(QWidget *parent = nullptr);
    QAction     *addTabAction;
};

#endif // ADDTAB_H
