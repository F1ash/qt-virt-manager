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

#include <QTabBar>
#include <QInputDialog>
#include <QMouseEvent>
#include <QMenu>

#include "termwidgetholder.h"
#include "tabwidget.h"
#include "config.h"
#include "properties.h"


#define TAB_INDEX_PROPERTY "tab_index"


TabWidget::TabWidget(QWidget* parent, QString _name) :
    QTabWidget(parent), tabName(_name)
{
    setFocusPolicy(Qt::NoFocus);

    /* On Mac OS X this will look similar to
     * the tabs in Safari or Leopard's Terminal.app .
     * I love this!
     */
    setDocumentMode(true);

    tabBar()->setUsesScrollButtons(true);

    setTabsClosable(false);
    setMovable(true);
    setUsesScrollButtons(true);

    tabBar()->installEventFilter(this);
    tabBar()->setVisible(false);

    //connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(removeTab(int)));
}

TermWidgetHolder * TabWidget::terminalHolder()
{
    return reinterpret_cast<TermWidgetHolder*>(widget(currentIndex()));
}

void TabWidget::setWorkDirectory(const QString& dir)
{
    this->work_dir = dir;
}

int TabWidget::addNewTab(const QString & term_name)
{
    TermWidgetHolder *ch = terminalHolder();
    QString cwd(work_dir);
    if (Properties::Instance()->useCWD && ch)
    {
        cwd = ch->currentTerminal()->impl()->workingDirectory();
        if (cwd.isEmpty())
            cwd = work_dir;
    }

    TermWidgetHolder *console = new TermWidgetHolder(cwd, QString(), this);
    connect(console, SIGNAL(finished()), SLOT(removeFinished()));
    //connect(console, SIGNAL(lastTerminalClosed()), this, SLOT(removeCurrentTab()));
    connect(console, SIGNAL(lastTerminalClosed()), this, SLOT(removeFinished()));

    int index = addTab(console, term_name);
    setCurrentIndex(index);
    console->setInitialFocus();

    return index;
}

void TabWidget::copySelection()
{
    terminalHolder()->currentTerminal()->impl()->copyClipboard();
}

void TabWidget::pasteClipboard()
{
    terminalHolder()->currentTerminal()->impl()->pasteClipboard();
}

void TabWidget::pasteSelection()
{
    terminalHolder()->currentTerminal()->impl()->pasteSelection();
}

void TabWidget::zoomIn()
{
    terminalHolder()->currentTerminal()->impl()->zoomIn();
}

void TabWidget::zoomOut()
{
    terminalHolder()->currentTerminal()->impl()->zoomOut();
}

void TabWidget::zoomReset()
{
    terminalHolder()->currentTerminal()->impl()->zoomReset();
}

void TabWidget::renameSession()
{
    // fake method;
}

void TabWidget::removeFinished()
{
    QObject* term = sender();
    QVariant prop = term->property(TAB_INDEX_PROPERTY);
    if(prop.isValid() && prop.canConvert(QVariant::Int))
    {
        int index = prop.toInt();
        removeTab(index);
//        if (count() == 0)
//            emit closeTabNotification();
    }
}

void TabWidget::removeTab(int index)
{
    setUpdatesEnabled(false);

    QWidget * w = widget(index);
    QTabWidget::removeTab(index);
    w->deleteLater();

    int current = currentIndex();
    if (current >= 0 )
    {
        qobject_cast<TermWidgetHolder*>(widget(current))->setInitialFocus();
    }
// do not decrease it as renaming is disabled in renameTabsAfterRemove
//    tabName--;
    setUpdatesEnabled(true);

    if (count() == 0)
        emit closeTabNotification();
}

void TabWidget::removeCurrentTab()
{
    // question disabled due user requests. Yes I agree it was anoying.
//    if (QMessageBox::question(this,
//                    tr("Close current session"),
//                    tr("Are you sure you want to close current sesstion?"),
//                    QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
//    {
    if (count() > 1) {
        removeTab(currentIndex());
    } else {
        emit closeTabNotification();
    }
}

void TabWidget::changeScrollPosition(QAction *triggered)
{
    QActionGroup *scrollPosition = static_cast<QActionGroup *>(sender());
    if(!scrollPosition)
        qFatal("scrollPosition is NULL");


    Properties::Instance()->scrollBarPos =
            scrollPosition->actions().indexOf(triggered);

    Properties::Instance()->saveSettings();
    propertiesChanged();

}

void TabWidget::changeTabPosition(QAction *triggered)
{
    QActionGroup *tabPosition = static_cast<QActionGroup *>(sender());
    if(!tabPosition)
        qFatal("tabPosition is NULL");

    Properties *prop = Properties::Instance();
    /* order is dictated from mainwindow.cpp */
    QTabWidget::TabPosition position =
            (QTabWidget::TabPosition)tabPosition->actions()
            .indexOf(triggered);
    setTabPosition(position);
    prop->tabsPos = position;
    prop->saveSettings();
    return;
}

void TabWidget::propertiesChanged()
{
    for (int i = 0; i < count(); ++i)
    {
        TermWidgetHolder *console = static_cast<TermWidgetHolder*>(widget(i));
        if ( console!=Q_NULLPTR ) console->propertiesChanged();
    }
}

void TabWidget::clearActiveTerminal()
{
    reinterpret_cast<TermWidgetHolder*>(widget(currentIndex()))->clearActiveTerminal();
}
