#ifndef VIRTNET_TOOLBAR_H
#define VIRTNET_TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QIcon>
#include "open_file_menu.h"
#include <QDebug>

class VirtNetToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit VirtNetToolBar(QWidget *parent = 0);
    ~VirtNetToolBar();

signals:
    void fileForMethod(const QStringList&);
    void execMethod(const QStringList&);

private:
    QAction         *start_Action;
    QAction         *destroy_Action;
    OpenFileMenu    *create_Menu;
    OpenFileMenu    *define_Menu;
    QAction         *create_Action;
    QAction         *define_Action;
    QAction         *undefine_Action;
    QAction         *setAutostart_Action;

private slots:
    void repeatParameters(QStringList&);
    void showHoveredMenu();
    void detectTriggerredAction(QAction*);

public slots:
    Qt::ToolBarArea get_ToolBarArea(int i) const;

};

#endif // VIRTNET_TOOLBAR_H
