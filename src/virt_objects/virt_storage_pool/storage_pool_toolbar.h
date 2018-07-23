#ifndef STORAGE_POOL_TOOLBAR_H
#define STORAGE_POOL_TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QPushButton>
#include <QTimerEvent>
#include <QSettings>
#include "virt_objects/open_file_menu.h"
//#include "delete_pool_menu.h"
//#include <QDebug>

class StoragePoolToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit StoragePoolToolBar(QWidget *parent = Q_NULLPTR);
    ~StoragePoolToolBar();

signals:
    void             fileForMethod(const Act_Param&);
    void             execMethod(const Act_Param&);

private:
    QAction         *start_Action;
    QAction         *destroy_Action;
    OpenFileMenu    *create_Menu;
    OpenFileMenu    *define_Menu;
    QAction         *create_Action;
    QAction         *define_Action;
    QAction         *undefine_Action;
    //QAction         *setAutostart_Action;
    //QAction         *delete_Action;
    //QAction         *getXMLDesc_Action;
    QAction         *overview_Action;
    //Delete_Pool_Menu
    //                *delete_Menu;
    QPushButton     *_autoReload;
    QAction         *autoReload;

    QSettings        settings;
    int              interval;
    int              timerId;

private slots:
    void             timerEvent(QTimerEvent*);
    void             showHoveredMenu();
    void             showMenu();
    void             detectTriggeredAction(QAction*);
    void             changeAutoReloadState(bool);

public slots:
    Qt::ToolBarArea  get_ToolBarArea(int) const;
    void             enableAutoReload();
    void             stopProcessing();
    bool             getAutoReloadState() const;
};

#endif // STORAGE_POOL_TOOLBAR_H
