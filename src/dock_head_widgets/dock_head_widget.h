#ifndef DOCK_HEAD_WIDGET_H
#define DOCK_HEAD_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QIcon>
#include <QPushButton>
#include <QVBoxLayout>
#include "bar_name.h"

class DockHeadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DockHeadWidget(
            QWidget *parent = 0,
            const QString = QString());

signals:
    void             floatChanged(bool);

private:
    const QString    name;
    QLabel          *icon;
    BarNameLabel    *nameWdg;
    QPushButton     *floatIt;
    QVBoxLayout     *commonLayout;

public slots:
    void             setTabBarName(const QString&);
    void             floatStateChanged(bool);
};

#endif // DOCK_HEAD_WIDGET_H
