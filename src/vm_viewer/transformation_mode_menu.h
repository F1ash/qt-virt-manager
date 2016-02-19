#ifndef TRANSFORMATION_MODE_MENU_H
#define TRANSFORMATION_MODE_MENU_H

#include <QMenu>
#include <QActionGroup>

class TransformationModeMenu : public QMenu
{
    Q_OBJECT
public:
    explicit TransformationModeMenu(
            QWidget *parent = nullptr,
            Qt::TransformationMode
                     _mode  = Qt::SmoothTransformation);

private:
    QAction         *smooth, *fast;
    QActionGroup    *act_group;
    Qt::TransformationMode
                     mode;

private slots:
    void             emitNewMode(QAction*);

public slots :
    Qt::TransformationMode
                     getMode() const;
};

#endif // TRANSFORMATION_MODE_MENU_H
