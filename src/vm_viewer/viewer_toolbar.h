#ifndef VIEWER_TOOLBAR_H
#define VIEWER_TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QMenu>
#include <QTimerEvent>
#include <QMoveEvent>
#include "vm_state_widget.h"
#include "virt_objects/virt_entity_enums.h"
//#include <QDebug>

class ViewerToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit ViewerToolBar(QWidget *parent = nullptr);
    //QAction         *start_Action;
    QAction         *restore_Action;
    QMenu           *start_Menu;
    QAction         *destroy_Action;
    QMenu           *destroy_Menu;
    QAction         *save_Action;
    QAction         *pause_Action;
    QAction         *reboot_Action;
    QAction         *reset_Action;
    QAction         *shutdown_Action;
    QMenu           *snapshot_Menu;
    QAction         *createSnapshot;
    QAction         *moreSnapshot_Actions;
    QAction         *snapshot_Action;
    QAction         *reconnect_Action;
    QAction         *keySeq_Action;
    QMenu           *keySequenceMenu;
    QAction         *sendKeySeq_BackSpc;
    QAction         *sendKeySeq_Del;
    QAction         *sendKeySeq_1;
    QAction         *sendKeySeq_2;
    QAction         *sendKeySeq_3;
    QAction         *sendKeySeq_4;
    QAction         *sendKeySeq_5;
    QAction         *sendKeySeq_6;
    QAction         *sendKeySeq_7;
    QAction         *sendKeySeq_8;
    QAction         *getScreenshot;
    QAction         *copyFiles_Action;
    QAction         *copyToClipboard;
    QAction         *pasteClipboard;
    QAction         *fullScreen;
    QAction         *scaled_to;
    VM_State_Widget *vm_stateWdg;
    QAction         *stateWdg_Action;
    QAction         *sep1, *sep2;

signals:
    void             execMethod(const Act_Param&);
    void             positionChanged(const QPoint&);

private:
    bool             downloadIsCompleted;

private slots:
    void             showMenu();
    void             detectTriggeredAction(QAction*);
    void             moveEvent(QMoveEvent*);

public slots:
    void             setCopyPasteState(bool);
    void             downloadCancelled();
    void             downloadCompleted();
};

#endif // VIEWER_TOOLBAR_H
