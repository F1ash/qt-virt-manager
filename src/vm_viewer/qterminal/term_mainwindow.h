
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "ui_qterminal.h"
#include "config.h"
#include "termwidgetholder.h"
#include "vm_viewer/vm_viewer.h"

class TermMainWindow : public VM_Viewer , private Ui::mainWindow
{
    Q_OBJECT

public:
    TermMainWindow(
            QWidget        *parent     = nullptr,
            virConnectPtr*  connPtrPtr = nullptr,
            QString         arg1       = QString(),
            QString         arg2       = QString(),
            const QString&  work_dir   = nullptr,
            const QString&  command    = nullptr);
    ~TermMainWindow();
    int getPtySlaveFd() const;
    TermWidget* getCurrentTerminal() const;

protected:
     bool event(QEvent* event);

private:
    QActionGroup *tabPosition, *scrollBarPosition;
    QMenu *tabPosMenu, *scrollPosMenu;

    QAction *toggleBorder, *renameSession;

    QString m_initWorkDir;
    QString m_initShell;

    QDockWidget *m_bookmarksDock;

    void migrate_settings();

    void setup_FileMenu_Actions();
    void setup_ActionsMenu_Actions();
    void setup_ViewMenu_Actions();

private slots:
    void on_consoleTabulator_currentChanged(int);
    void propertiesChanged();
    void actAbout_triggered();
    void actProperties_triggered();
    void updateActionGroup(QAction *);

    void toggleBorderless();
    void toggleMenu();

    void showHide();
    void find();

    void bookmarksWidget_callCommand(const QString&);
    void bookmarksDock_visibilityChanged(bool visible);

    void addNewTab();
};
#endif //MAIN_WINDOW_H
