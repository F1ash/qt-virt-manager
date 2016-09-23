#ifndef VIEW_MENU_H
#define VIEW_MENU_H

#include <QMenu>

class ViewMenu : public QMenu
{
    Q_OBJECT
public:
    explicit ViewMenu(QWidget *parent = nullptr);
    QAction         *hardClassic;
    QAction         *softTouched;

private:
    QActionGroup    *actGroup;

private slots:
    void             viewModeChanged();
};

#endif // VIEW_MENU_H
