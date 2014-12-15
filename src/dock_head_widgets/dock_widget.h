#ifndef DOCK_WIDGET_H
#define DOCK_WIDGET_H

#include <QDockWidget>

class DockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockWidget(QWidget *parent = NULL);

public slots:
    void _setFloating(bool);
};

#endif // DOCK_WIDGET_H
