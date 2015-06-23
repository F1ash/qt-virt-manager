#ifndef STORAGE_VOL_TOOLBAR_H
#define STORAGE_VOL_TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QPushButton>
#include <QTimerEvent>
#include <QSettings>
#include "virt_objects/open_file_menu.h"
#include "wipe_menu.h"
#include <QDebug>

class StorageVolToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit StorageVolToolBar(QWidget *parent = NULL);
    ~StorageVolToolBar();

signals:
    void             fileForMethod(const QStringList&);
    void             execMethod(const QStringList&);

private:
    OpenFileMenu    *create_Menu;
    QAction         *create_Action;
    QAction         *delete_Action;
    QAction         *download_Action;
    QAction         *resize_Action;
    QAction         *upload_Action;
    QAction         *wipe_Action;
    WipeMenu        *wipe_Menu;
    QAction         *getXMLDesc_Action;
    QPushButton     *_autoReload;
    QAction         *autoReload;

    QSettings        settings;
    int              interval;
    int              timerId = 0;

private slots:
    void             timerEvent(QTimerEvent*);
    void             repeatParameters(QStringList&);
    void             showMenu();
    void             detectTriggerredAction(QAction*);
    void             changeAutoReloadState(bool);

public slots:
    Qt::ToolBarArea  get_ToolBarArea(int) const;
    void             enableAutoReload();
    void             stopProcessing();
    bool             getAutoReloadState() const;
};

#endif // STORAGE_VOL_TOOLBAR_H
