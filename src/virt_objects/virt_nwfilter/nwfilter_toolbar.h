#ifndef VIRT_NWFILTER_TOOLBAR_H
#define VIRT_NWFILTER_TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QPushButton>
#include <QTimerEvent>
#include <QSettings>
#include "virt_objects/open_file_menu.h"
//#include <QDebug>

class VirtNWFilterToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit VirtNWFilterToolBar(QWidget *parent = nullptr);
    ~VirtNWFilterToolBar();

signals:
    void             fileForMethod(const Act_Param&);
    void             execMethod(const Act_Param&);

private:
    OpenFileMenu    *define_Menu;
    QAction         *define_Action;
    QAction         *undefine_Action;
    //QAction         *getXMLDesc_Action;
    QPushButton     *_autoReload;
    QAction         *autoReload;

    QSettings        settings;
    int              interval;
    int              timerId;

private slots:
    void             timerEvent(QTimerEvent*);
    void             showMenu();
    void             detectTriggeredAction(QAction*);
    void             changeAutoReloadState(bool);

public slots:
    Qt::ToolBarArea  get_ToolBarArea(int) const;
    void             enableAutoReload();
    void             stopProcessing();
    bool             getAutoReloadState() const;

};

#endif // VIRT_NWFILTER_TOOLBAR_H
