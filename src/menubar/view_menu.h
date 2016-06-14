#ifndef VIEW_MENU_H
#define VIEW_MENU_H

#include <QMenu>

class ViewMenu : public QMenu
{
    Q_OBJECT
public:
    explicit ViewMenu(QWidget *parent = nullptr);
    QAction         *hardClassic;
    QAction         *gallantSoft;

private:
    QActionGroup    *actGroup;
};

#endif // VIEW_MENU_H
