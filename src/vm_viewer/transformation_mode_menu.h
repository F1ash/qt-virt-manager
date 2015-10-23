#ifndef TRANSFORMATION_MODE_MENU_H
#define TRANSFORMATION_MODE_MENU_H

#include <QMenu>
#include <QActionGroup>

class TransformationModeMenu : public QMenu
{
    Q_OBJECT
public:
    explicit TransformationModeMenu(QWidget *parent = NULL);

signals:
    void             new_mode(Qt::TransformationMode);

private:
    QAction         *smooth, *fast;
    QActionGroup    *act_group;

private slots:
    void             emitNewMode(QAction*);
};

#endif // TRANSFORMATION_MODE_MENU_H
