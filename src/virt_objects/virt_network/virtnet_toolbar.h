#ifndef VIRTNET_TOOLBAR_H
#define VIRTNET_TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QPushButton>
#include <QTimerEvent>
#include <QSettings>
#include "virt_objects/open_file_menu.h"
#include <QDebug>

class VirtNetToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit VirtNetToolBar(QWidget *parent = nullptr);
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
    //QAction         *setAutostart_Action;
    //QAction         *getXMLDesc_Action;
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

#endif // VIRTNET_TOOLBAR_H
