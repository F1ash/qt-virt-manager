#ifndef STORAGE_VOL_TOOLBAR_H
#define STORAGE_VOL_TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QIcon>
#include "virt_objects/open_file_menu.h"
#include <QDebug>

class StorageVolToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit StorageVolToolBar(QWidget *parent = 0);
    ~StorageVolToolBar();

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
    QAction         *getXMLDesc_Action;
    QAction         *stopOverview_Action;

private slots:
    void repeatParameters(QStringList&);
    void showHoveredMenu();
    void showMenu();
    void detectTriggerredAction(QAction*);

public slots:
    Qt::ToolBarArea get_ToolBarArea(int) const;

};

#endif // STORAGE_VOL_TOOLBAR_H