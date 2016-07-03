#ifndef ADD_TAB_H
#define ADD_TAB_H

#include <QMenu>

class AddTab : public QMenu
{
    Q_OBJECT
public:
    explicit AddTab(QWidget *parent = nullptr);
    QAction     *addTabAction;
};

#endif // ADD_TAB_H
