#ifndef DOCK_HEAD_WIDGET_H
#define DOCK_HEAD_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QIcon>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWheelEvent>
#include "bar_name.h"
//#include <QDebug>

class DockHeadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DockHeadWidget(
            QWidget *parent = nullptr,
            const QString = QString());
    void            setFloatible(bool);

signals:
    void             floatChanged(bool);
    void             viewNextDock();
    void             viewPrevDock();

private:
    const QString    name;
    QLabel          *icon;
    BarNameLabel    *nameWdg;
    QPushButton     *floatIt;
    QVBoxLayout     *commonLayout;

public slots:
    void             setTabBarName(const QString&);
    void             floatStateChanged(bool);

private slots:
    void             wheelEvent(QWheelEvent*);
};

#endif // DOCK_HEAD_WIDGET_H
