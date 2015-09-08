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
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QDockWidget>
#include <QDesktopWidget>
#include <QMessageBox>

#include "term_mainwindow.h"
#include "tabwidget.h"
#include "properties.h"
#include "propertiesdialog.h"
#include "bookmarkswidget.h"


// TODO/FXIME: probably remove. QSS makes it unusable on mac...
#define QSS_DROP    "TermMainWindow {border: 1px solid rgba(0, 0, 0, 50%);}\n"

TermMainWindow::TermMainWindow(QWidget *parent,
        virConnect *conn,
        QString arg1,
        QString arg2,
        const QString& work_dir,
        const QString& command) :
      VM_Viewer(parent, conn, arg1, arg2),
      m_initShell(command),
      m_initWorkDir(work_dir)
{
    setupUi(this);
    Properties::Instance()->loadSettings();
    migrate_settings();

    m_bookmarksDock = new QDockWidget(tr("Bookmarks"), this);
    m_bookmarksDock->setObjectName("BookmarksDockWidget");
    BookmarksWidget *bookmarksWidget = new BookmarksWidget(m_bookmarksDock);
    m_bookmarksDock->setWidget(bookmarksWidget);
    addDockWidget(Qt::LeftDockWidgetArea, m_bookmarksDock);
    connect(bookmarksWidget, SIGNAL(callCommand(QString)),
            this, SLOT(bookmarksWidget_callCommand(QString)));

    connect(m_bookmarksDock, SIGNAL(visibilityChanged(bool)),
            this, SLOT(bookmarksDock_visibilityChanged(bool)));

    connect(actAbout, SIGNAL(triggered()), SLOT(actAbout_triggered()));
    connect(actAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    setContentsMargins(0, 0, 0, 0);
    restoreGeometry(Properties::Instance()->mainWindowGeometry);
    restoreState(Properties::Instance()->mainWindowState);

    connect(consoleTabulator, SIGNAL(closeTabNotification()), SLOT(close()));
    consoleTabulator->setWorkDirectory(work_dir);
    consoleTabulator->setTabPosition((QTabWidget::TabPosition)Properties::Instance()->tabsPos);
    //consoleTabulator->setShellProgram(command);
    consoleTabulator->addNewTab(QString("<%2> Domain Terminal in [ %1 ] Connection").arg(arg1).arg(arg2));

    setWindowTitle(QString("<%2> Domain Terminal in <%1> Connection").arg(arg1).arg(arg2));
    setWindowIcon(QIcon::fromTheme("utilities-terminal"));

    setup_FileMenu_Actions();
    setup_ActionsMenu_Actions();
    setup_ViewMenu_Actions();

    // Add global rename Session shortcut
    renameSession = new QAction(tr("Rename Session"), this);
    renameSession->setShortcut(QKeySequence(tr(RENAME_SESSION_SHORTCUT)));
    connect(renameSession, SIGNAL(triggered()), consoleTabulator, SLOT(renameSession()));
}

TermMainWindow::~TermMainWindow()
{
}

void TermMainWindow::migrate_settings()
{
    // Deal with rearrangements of settings.
    // If this method becomes unbearably huge we should look at the config-update
    // system used by kde and razor.
    QSettings settings;
    QString last_version = settings.value("version", "0.0.0").toString();
    // Handle configchanges in 0.4.0 (renaming 'Paste Selection' -> 'Paste Clipboard')
    if (last_version < "0.4.0")
    {
        qDebug() << "Migrating settings from" << last_version << "to 0.4.0";
        settings.beginGroup("Shortcuts");
        QString tmp = settings.value("Paste Selection", PASTE_CLIPBOARD_SHORTCUT).toString();
        settings.setValue(PASTE_CLIPBOARD, tmp);
        settings.remove("Paste Selection");
        settings.endGroup();

        settings.setValue("version", "0.4.0");
    }
}

void TermMainWindow::setup_ActionsMenu_Actions()
{
    QSettings settings;
    settings.beginGroup("Shortcuts");

    QKeySequence seq;

    Properties::Instance()->actions[CLEAR_TERMINAL] = new QAction(QIcon::fromTheme("edit-clear"), tr("Clear Current Tab"), this);
    seq = QKeySequence::fromString(settings.value(CLEAR_TERMINAL, CLEAR_TERMINAL_SHORTCUT).toString());
    Properties::Instance()->actions[CLEAR_TERMINAL]->setShortcut(seq);
    connect(Properties::Instance()->actions[CLEAR_TERMINAL], SIGNAL(triggered()), consoleTabulator, SLOT(clearActiveTerminal()));
    menu_Actions->addAction(Properties::Instance()->actions[CLEAR_TERMINAL]);

    menu_Actions->addSeparator();

    // Copy and Paste are only added to the table for the sake of bindings at the moment; there is no Edit menu, only a context menu.
    Properties::Instance()->actions[COPY_SELECTION] = new QAction(QIcon::fromTheme("edit-copy"), tr("Copy Selection"), this);
    seq = QKeySequence::fromString( settings.value(COPY_SELECTION, COPY_SELECTION_SHORTCUT).toString() );
    Properties::Instance()->actions[COPY_SELECTION]->setShortcut(seq);
    connect(Properties::Instance()->actions[COPY_SELECTION], SIGNAL(triggered()), consoleTabulator, SLOT(copySelection()));
    menu_Edit->addAction(Properties::Instance()->actions[COPY_SELECTION]);

    Properties::Instance()->actions[PASTE_CLIPBOARD] = new QAction(QIcon::fromTheme("edit-paste"), tr("Paste Clipboard"), this);
    seq = QKeySequence::fromString( settings.value(PASTE_CLIPBOARD, PASTE_CLIPBOARD_SHORTCUT).toString() );
    Properties::Instance()->actions[PASTE_CLIPBOARD]->setShortcut(seq);
    connect(Properties::Instance()->actions[PASTE_CLIPBOARD], SIGNAL(triggered()), consoleTabulator, SLOT(pasteClipboard()));
    menu_Edit->addAction(Properties::Instance()->actions[PASTE_CLIPBOARD]);

    Properties::Instance()->actions[PASTE_SELECTION] = new QAction(QIcon::fromTheme("edit-paste"), tr("Paste Selection"), this);
    seq = QKeySequence::fromString( settings.value(PASTE_SELECTION, PASTE_SELECTION_SHORTCUT).toString() );
    Properties::Instance()->actions[PASTE_SELECTION]->setShortcut(seq);
    connect(Properties::Instance()->actions[PASTE_SELECTION], SIGNAL(triggered()), consoleTabulator, SLOT(pasteSelection()));
    menu_Edit->addAction(Properties::Instance()->actions[PASTE_SELECTION]);

    menu_Actions->addSeparator();

    Properties::Instance()->actions[ZOOM_IN] = new QAction(QIcon::fromTheme("zoom-in"), tr("Zoom in"), this);
    seq = QKeySequence::fromString( settings.value(ZOOM_IN, ZOOM_IN_SHORTCUT).toString() );
    Properties::Instance()->actions[ZOOM_IN]->setShortcut(seq);
    connect(Properties::Instance()->actions[ZOOM_IN], SIGNAL(triggered()), consoleTabulator, SLOT(zoomIn()));
    menu_Edit->addAction(Properties::Instance()->actions[ZOOM_IN]);

    Properties::Instance()->actions[ZOOM_OUT] = new QAction(QIcon::fromTheme("zoom-out"), tr("Zoom out"), this);
    seq = QKeySequence::fromString( settings.value(ZOOM_OUT, ZOOM_OUT_SHORTCUT).toString() );
    Properties::Instance()->actions[ZOOM_OUT]->setShortcut(seq);
    connect(Properties::Instance()->actions[ZOOM_OUT], SIGNAL(triggered()), consoleTabulator, SLOT(zoomOut()));
    menu_Edit->addAction(Properties::Instance()->actions[ZOOM_OUT]);

    Properties::Instance()->actions[ZOOM_RESET] = new QAction(QIcon::fromTheme("zoom-original"), tr("Zoom reset"), this);
    seq = QKeySequence::fromString( settings.value(ZOOM_RESET, ZOOM_RESET_SHORTCUT).toString() );
    Properties::Instance()->actions[ZOOM_RESET]->setShortcut(seq);
    connect(Properties::Instance()->actions[ZOOM_RESET], SIGNAL(triggered()), consoleTabulator, SLOT(zoomReset()));
    menu_Edit->addAction(Properties::Instance()->actions[ZOOM_RESET]);

    menu_Actions->addSeparator();

    Properties::Instance()->actions[FIND] = new QAction(QIcon::fromTheme("edit-find"), tr("Find..."), this);
    seq = QKeySequence::fromString( settings.value(FIND, FIND_SHORTCUT).toString() );
    Properties::Instance()->actions[FIND]->setShortcut(seq);
    connect(Properties::Instance()->actions[FIND], SIGNAL(triggered()), this, SLOT(find()));
    menu_Actions->addAction(Properties::Instance()->actions[FIND]);

    Properties::Instance()->actions[TOGGLE_MENU] = new QAction(tr("Toggle Menu"), this);
    seq = QKeySequence::fromString( settings.value(TOGGLE_MENU, TOGGLE_MENU_SHORTCUT).toString() );
    Properties::Instance()->actions[TOGGLE_MENU]->setShortcut(seq);
    connect(Properties::Instance()->actions[TOGGLE_MENU], SIGNAL(triggered()), this, SLOT(toggleMenu()));
    // tis is correct - add action to main window - not to menu to keep toggle working

    settings.endGroup();

    // apply props
    propertiesChanged();
}
void TermMainWindow::setup_FileMenu_Actions()
{
    QSettings settings;
    settings.beginGroup("Shortcuts");

    Properties::Instance()->actions[PREFERENCES] = actProperties;
    connect(actProperties, SIGNAL(triggered()), SLOT(actProperties_triggered()));
    menu_File->addAction(Properties::Instance()->actions[PREFERENCES]);

    menu_File->addSeparator();

    Properties::Instance()->actions[QUIT] = actQuit;
    connect(actQuit, SIGNAL(triggered()), SLOT(close()));
    menu_File->addAction(Properties::Instance()->actions[QUIT]);

    settings.endGroup();
}

void TermMainWindow::setup_ViewMenu_Actions()
{
    toggleBorder = new QAction(tr("Hide Window Borders"), this);
    //toggleBorder->setObjectName("toggle_Borderless");
    toggleBorder->setCheckable(true);
// TODO/FIXME: it's broken somehow. When I call toggleBorderless() here the non-responsive window appear
//    toggleBorder->setChecked(Properties::Instance()->borderless);
//    if (Properties::Instance()->borderless)
//        toggleBorderless();
    connect(toggleBorder, SIGNAL(triggered()), this, SLOT(toggleBorderless()));
    menu_Window->addAction(toggleBorder);
    toggleBorder->setVisible(true);

    QSettings settings;
    settings.beginGroup("Shortcuts");
    Properties::Instance()->actions[TOGGLE_BOOKMARKS] = m_bookmarksDock->toggleViewAction();
    QKeySequence seq = QKeySequence::fromString( settings.value(TOGGLE_BOOKMARKS, TOGGLE_BOOKMARKS_SHORTCUT).toString() );
    Properties::Instance()->actions[TOGGLE_BOOKMARKS]->setShortcut(seq);
    menu_Window->addAction(Properties::Instance()->actions[TOGGLE_BOOKMARKS]);
    settings.endGroup();

    menu_Window->addSeparator();

    /* Scrollbar */
    scrollBarPosition = new QActionGroup(this);
    QAction *scrollNone = new QAction(tr("None"), this);
    QAction *scrollRight = new QAction(tr("Right"), this);
    QAction *scrollLeft = new QAction(tr("Left"), this);

    /* order of insertion is dep. on QTermWidget::ScrollBarPosition enum */
    scrollBarPosition->addAction(scrollNone);
    scrollBarPosition->addAction(scrollLeft);
    scrollBarPosition->addAction(scrollRight);

    for(int i = 0; i < scrollBarPosition->actions().size(); ++i)
        scrollBarPosition->actions().at(i)->setCheckable(true);

    if( Properties::Instance()->scrollBarPos < scrollBarPosition->actions().size() )
        scrollBarPosition->actions().at(Properties::Instance()->scrollBarPos)->setChecked(true);

    connect(scrollBarPosition, SIGNAL(triggered(QAction *)),
             consoleTabulator, SLOT(changeScrollPosition(QAction *)) );

    scrollPosMenu = new QMenu(tr("Scrollbar Layout"), menu_Window);
    scrollPosMenu->setObjectName("scrollPosMenu");

    for(int i=0; i < scrollBarPosition->actions().size(); ++i) {
        scrollPosMenu->addAction(scrollBarPosition->actions().at(i));
    }

    menu_Window->addMenu(scrollPosMenu);
}

void TermMainWindow::on_consoleTabulator_currentChanged(int)
{
}

void TermMainWindow::toggleBorderless()
{
    setWindowFlags(windowFlags() ^ Qt::FramelessWindowHint);
    show();
    setWindowState(Qt::WindowActive); /* don't loose focus on the window */
    Properties::Instance()->borderless = toggleBorder->isChecked();
}

void TermMainWindow::toggleMenu()
{
    m_menuBar->setVisible(!m_menuBar->isVisible());
    Properties::Instance()->menuVisible = m_menuBar->isVisible();
}

void TermMainWindow::actAbout_triggered()
{
    QMessageBox::about(this, QString("QTerminal "), tr("A lightweight multiplatform terminal emulator"));
}

void TermMainWindow::actProperties_triggered()
{
    PropertiesDialog *p = new PropertiesDialog(this);
    connect(p, SIGNAL(propertiesChanged()), this, SLOT(propertiesChanged()));
    p->exec();
}

void TermMainWindow::propertiesChanged()
{
    QApplication::setStyle(Properties::Instance()->guiStyle);
    setWindowOpacity(Properties::Instance()->appOpacity/100.0);
    consoleTabulator->setTabPosition((QTabWidget::TabPosition)Properties::Instance()->tabsPos);
    consoleTabulator->propertiesChanged();

    m_menuBar->setVisible(Properties::Instance()->menuVisible);

    m_bookmarksDock->setVisible(Properties::Instance()->useBookmarks
                                && Properties::Instance()->bookmarksVisible);
    m_bookmarksDock->toggleViewAction()->setVisible(Properties::Instance()->useBookmarks);

    if (Properties::Instance()->useBookmarks)
    {
        qobject_cast<BookmarksWidget*>(m_bookmarksDock->widget())->setup();
    }

    Properties::Instance()->saveSettings();
}

void TermMainWindow::updateActionGroup(QAction *a)
{
    if (a->parent()->objectName() == tabPosMenu->objectName()) {
        tabPosition->actions().at(Properties::Instance()->tabsPos)->setChecked(true);
    }
}

void TermMainWindow::showHide()
{
    if (isVisible())
        hide();
    else
    {
       show();
       activateWindow();
    }
}

void TermMainWindow::find()
{
    // A bit ugly perhaps with 4 levels of indirection...
    consoleTabulator->terminalHolder()->currentTerminal()->impl()->toggleShowSearchBar();
}

bool TermMainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::WindowDeactivate)
    {
        if (false &&
            !Properties::Instance()->dropKeepOpen &&
            qApp->activeWindow() == 0
           )
           hide();
    }
    return QMainWindow::event(event);
}

void TermMainWindow::bookmarksWidget_callCommand(const QString& cmd)
{
    consoleTabulator->terminalHolder()->currentTerminal()->impl()->sendText(cmd);
    consoleTabulator->terminalHolder()->currentTerminal()->setFocus();
}

void TermMainWindow::bookmarksDock_visibilityChanged(bool visible)
{
    Properties::Instance()->bookmarksVisible = visible;
}

void TermMainWindow::addNewTab()
{
    consoleTabulator->addNewTab();
}

int TermMainWindow::getPtySlaveFd() const
{
    TermWidget *t = consoleTabulator->terminalHolder()->currentTerminal();
    if ( NULL!=t ) return t->getPtySlaveFd();
    else return -1;
}

TermWidget* TermMainWindow::getCurrentTerminal() const
{
    return consoleTabulator->terminalHolder()->currentTerminal();
}
