#ifndef DOMAIN_TOOLBAR_H
#define DOMAIN_TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QPushButton>
#include <QTimerEvent>
#include <QSettings>
#include "virt_objects/open_file_menu.h"
#include <QDebug>

class DomainToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit DomainToolBar(QWidget *parent = NULL);
    ~DomainToolBar();

signals:
    void fileForMethod(const QStringList&);
    void execMethod(const QStringList&);

private:
    QAction         *start_Action;
    QAction         *restore_Action;
    QMenu           *start_Menu;
    QAction         *destroy_Action;
    QMenu           *destroy_Menu;
    QAction         *save_Action;
    QAction         *sep;
    QAction         *pause_Action;
    QAction         *reboot_Action;
    QAction         *reset_Action;
    QAction         *shutdown_Action;
    OpenFileMenu    *create_Menu;
    OpenFileMenu    *define_Menu;
    QAction         *create_Action;
    QAction         *define_Action;
    QAction         *undefine_Action;
    QAction         *setAutostart_Action;
    QAction         *getXMLDesc_Action;
    QAction         *migrate_Action;
    QPushButton     *_autoReload;
    QAction         *autoReload;

    QSettings        settings;
    int              interval;
    int              timerId;

private slots:
    void timerEvent(QTimerEvent*);
    void repeatParameters(QStringList&);
    void showHoveredMenu();
    void showMenu();
    void detectTriggerredAction(QAction*);
    void changeAutoReloadState(bool);

public slots:
    Qt::ToolBarArea get_ToolBarArea(int) const;
    void enableAutoReload();
    void stopProcessing();
    bool getAutoReloadState() const;

};

#endif // DOMAIN_TOOLBAR_H
