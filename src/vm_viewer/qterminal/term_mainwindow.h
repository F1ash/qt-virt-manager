/***************************************************************************
 *   Copyright (C) 2006 by Vladimir Kuznetsov                              *
 *   vovanec@gmail.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

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
            QWidget        *parent     = Q_NULLPTR,
            virConnectPtr*  connPtrPtr = Q_NULLPTR,
            QString         arg1       = QString(),
            QString         arg2       = QString(),
            const QString&  work_dir   = Q_NULLPTR,
            const QString&  command    = Q_NULLPTR);
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
