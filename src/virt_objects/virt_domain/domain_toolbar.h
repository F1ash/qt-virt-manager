#ifndef DOMAIN_TOOLBAR_H
#define DOMAIN_TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QIcon>
#include "virt_objects/open_file_menu.h"
#include <QDebug>

class DomainToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit DomainToolBar(QWidget *parent = 0);
    ~DomainToolBar();

signals:
    void fileForMethod(const QStringList&);
    void execMethod(const QStringList&);

private:
    QAction         *start_Action;
    QAction         *destroy_Action;
    QMenu           *destroy_Menu;
    QAction         *restore_Action;
    QAction         *save_Action;
    QAction         *sep;
    QAction         *resume_Action;
    QAction         *suspend_Action;
    QAction         *reboot_Action;
    QAction         *reset_Action;
    OpenFileMenu    *create_Menu;
    OpenFileMenu    *define_Menu;
    QAction         *create_Action;
    QAction         *define_Action;
    QAction         *undefine_Action;
    QAction         *setAutostart_Action;
    QAction         *getXMLDesc_Action;

private slots:
    void repeatParameters(QStringList&);
    void showHoveredMenu();
    void showMenu();
    void detectTriggerredAction(QAction*);

public slots:
    Qt::ToolBarArea get_ToolBarArea(int i) const;

};

#endif // DOMAIN_TOOLBAR_H
