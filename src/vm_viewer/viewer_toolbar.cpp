#include "viewer_toolbar.h"

ViewerToolBar::ViewerToolBar(QWidget *parent) :
    QToolBar(parent)
{
    downloadIsCompleted = true;
    //start_Action = new QAction(this);
    //start_Action->setIcon(QIcon::fromTheme("start"));
    //start_Action->setToolTip(tr("Start"));
    //start_Menu = new QMenu(this);
    //restore_Action = start_Menu->addAction(tr("Restore"));
    //restore_Action->setIcon(QIcon::fromTheme("start"));
    //start_Action->setMenu(start_Menu);
    pause_Action = new QAction(this);
    pause_Action->setIcon(QIcon::fromTheme("pause"));
    pause_Action->setToolTip(tr("Pause"));
    destroy_Menu = new QMenu(this);
    reboot_Action = destroy_Menu->addAction(tr("Reboot"));
    reboot_Action->setIcon(QIcon::fromTheme("reboot"));
    reset_Action = destroy_Menu->addAction(tr("Reset"));
    reset_Action->setIcon(QIcon::fromTheme("reset"));
    destroy_Menu->addSeparator();
    shutdown_Action = destroy_Menu->addAction(tr("Shutdown"));
    shutdown_Action->setIcon(QIcon::fromTheme("shutdown"));
    save_Action = destroy_Menu->addAction(tr("Save"));
    save_Action->setIcon(QIcon::fromTheme("save"));
    destroy_Action = new QAction(this);
    destroy_Action->setIcon(QIcon::fromTheme("destroy"));
    destroy_Action->setToolTip(tr("Stop"));
    destroy_Action->setMenu(destroy_Menu);
    snapshot_Menu = new QMenu(this);
    createSnapshot = snapshot_Menu->addAction(
                tr("create Snapshot of Current Domain"));
    createSnapshot->setIcon(QIcon::fromTheme("camera-photo"));
    moreSnapshot_Actions = snapshot_Menu->addAction(
                tr("more Snapshot actions for Domain"));
    moreSnapshot_Actions->setIcon(QIcon::fromTheme("camera-photo"));
    snapshot_Action = new QAction(this);
    snapshot_Action->setIcon(QIcon::fromTheme("camera-photo"));
    snapshot_Action->setToolTip(tr("Snapshot now!"));
    snapshot_Action->setMenu(snapshot_Menu);
    reconnect_Action = new QAction(this);
    reconnect_Action->setIcon(QIcon::fromTheme("view-refresh"));
    reconnect_Action->setToolTip(tr("Reconnect"));
    keySeq_Action = new QAction(this);
    keySeq_Action->setIcon(QIcon::fromTheme("input-keyboard"));
    keySeq_Action->setToolTip(tr("Send key sequence"));
    keySequenceMenu = new QMenu(this);
    keySeq_Action->setMenu(keySequenceMenu);
    sendKeySeq_BackSpc = keySequenceMenu->addAction("Ctrl+Alt+BackSpace");
    sendKeySeq_Del = keySequenceMenu->addAction("Ctrl+Alt+Del");
    keySequenceMenu->addSeparator();
    sendKeySeq_1 = keySequenceMenu->addAction("Ctrl+Alt+F1");
    sendKeySeq_2 = keySequenceMenu->addAction("Ctrl+Alt+F2");
    sendKeySeq_3 = keySequenceMenu->addAction("Ctrl+Alt+F3");
    sendKeySeq_4 = keySequenceMenu->addAction("Ctrl+Alt+F4");
    sendKeySeq_5 = keySequenceMenu->addAction("Ctrl+Alt+F5");
    sendKeySeq_6 = keySequenceMenu->addAction("Ctrl+Alt+F6");
    sendKeySeq_7 = keySequenceMenu->addAction("Ctrl+Alt+F7");
    sendKeySeq_8 = keySequenceMenu->addAction("Ctrl+Alt+F8");
    keySequenceMenu->addSeparator();
    getScreenshot = keySequenceMenu->addAction(tr("get Guest Screenshot"));
    copyFiles_Action = new QAction(this);
    copyFiles_Action->setIcon(QIcon::fromTheme("document-send"));
    copyFiles_Action->setToolTip(tr("Copy Files to Guest"));
    copyToClipboard = new QAction(this);
    copyToClipboard->setIcon(QIcon::fromTheme("edit-copy"));
    copyToClipboard->setToolTip(tr("Copy from Guest to Clipboard"));
    pasteClipboard = new QAction(this);
    pasteClipboard->setIcon(QIcon::fromTheme("edit-paste"));
    pasteClipboard->setToolTip(tr("Paste Clipboard to Guest"));
    fullScreen = new QAction(this);
    fullScreen->setIcon(QIcon::fromTheme("fullscreen"));
    fullScreen->setToolTip(tr("FullScreen"));
    scaled_to = new QAction(this);
    scaled_to->setIcon(QIcon::fromTheme("scaled_to_window"));
    scaled_to->setToolTip(tr("Scale to window"));
    vm_stateWdg = new VM_State_Widget(this);

    //addAction(start_Action);
    addAction(pause_Action);
    addAction(destroy_Action);
    sep1 = addSeparator();
    addAction(snapshot_Action);
    sep2 = addSeparator();
    addAction(reconnect_Action);
    addSeparator();
    addAction(keySeq_Action);
    addAction(copyFiles_Action);
    addAction(copyToClipboard);
    addAction(pasteClipboard);
    addSeparator();
    addAction(fullScreen);
    addAction(scaled_to);
    addSeparator();
    stateWdg_Action = addWidget(vm_stateWdg);

    connect(this, SIGNAL(actionTriggered(QAction*)),
            this, SLOT(detectTriggeredAction(QAction*)));
    setContentsMargins(0,0,0,0);
}

/* public slots */
void ViewerToolBar::setCopyPasteState(bool state)
{
    copyToClipboard->setEnabled(state);
    pasteClipboard->setEnabled(state);
}
void ViewerToolBar::downloadCancelled()
{
    if ( !downloadIsCompleted )
        copyFiles_Action->setEnabled(false);
}
void ViewerToolBar::downloadCompleted()
{
    downloadIsCompleted = true;
    copyFiles_Action->setIcon(QIcon::fromTheme("document-send"));
    copyFiles_Action->setToolTip(tr("Copy Files to Guest"));
    copyFiles_Action->setEnabled(true);
}

/* private slots */
void ViewerToolBar::showMenu()
{
    QAction *act = static_cast<QAction*>(sender());
    if ( act==nullptr ) return;
    if ( act->menu()->isVisible() ) act->menu()->hide();
    else {
        act->menu()->show();
        act->menu()->move(QCursor::pos());
    };
}
void ViewerToolBar::detectTriggeredAction(QAction *action)
{
    Act_Param parameters;
    //if ( action == start_Action) {
    //    parameters << "startVirtDomain";
    //} else
    if        ( action == pause_Action ) {
        parameters.method = Methods::pauseEntity;
    } else if ( action == destroy_Action ) {
        parameters.method = Methods::destroyEntity;
    } else if ( action == reset_Action ) {
        parameters.method = Methods::resetVirtDomain;
    } else if ( action == reboot_Action ) {
        parameters.method = Methods::rebootVirtDomain;
    } else if ( action == shutdown_Action ) {
        parameters.method = Methods::shutdownVirtDomain;
    } else if ( action == save_Action ) {
        parameters.method = Methods::saveVirtDomain;
    } else if ( action == restore_Action ) {
        parameters.method = Methods::restoreVirtDomain;
    } else if ( action == snapshot_Action ) {
        parameters.method = Methods::createVirtDomainSnapshot;
    } else if ( action == createSnapshot ) {
        parameters.method = Methods::createVirtDomainSnapshot;
    } else if ( action == moreSnapshot_Actions ) {
        parameters.method = Methods::moreSnapshotActions;
    } else if ( action == reconnect_Action ) {
        parameters.method = Methods::reconnectToVirtDomain;
    } else if ( action == sendKeySeq_1 ) {
        parameters.method = Methods::sendKeySeqToVirtDomain;
        parameters.path = QString::number(Qt::Key_Launch1);
    } else if ( action == sendKeySeq_2 ) {
        parameters.method = Methods::sendKeySeqToVirtDomain;
        parameters.path = QString::number(Qt::Key_Launch2);
    } else if ( action == sendKeySeq_3 ) {
        parameters.method = Methods::sendKeySeqToVirtDomain;
        parameters.path = QString::number(Qt::Key_Launch3);
    } else if ( action == sendKeySeq_4 ) {
        parameters.method = Methods::sendKeySeqToVirtDomain;
        parameters.path = QString::number(Qt::Key_Launch4);
    } else if ( action == sendKeySeq_5 ) {
        parameters.method = Methods::sendKeySeqToVirtDomain;
        parameters.path = QString::number(Qt::Key_Launch5);
    } else if ( action == sendKeySeq_6 ) {
        parameters.method = Methods::sendKeySeqToVirtDomain;
        parameters.path = QString::number(Qt::Key_Launch6);
    } else if ( action == sendKeySeq_7 ) {
        parameters.method = Methods::sendKeySeqToVirtDomain;
        parameters.path = QString::number(Qt::Key_Launch7);
    } else if ( action == sendKeySeq_8 ) {
        parameters.method = Methods::sendKeySeqToVirtDomain;
        parameters.path = QString::number(Qt::Key_Launch8);
    } else if ( action == getScreenshot ) {
        parameters.method = Methods::getScreenshotFromVirtDomain;
    } else if ( action == sendKeySeq_BackSpc ) {
        parameters.method = Methods::sendKeySeqToVirtDomain;
        parameters.path = QString::number(Qt::Key_LaunchB);
    } else if ( action == sendKeySeq_Del ) {
        parameters.method = Methods::sendKeySeqToVirtDomain;
        parameters.path = QString::number(Qt::Key_LaunchD);
    } else if ( action == copyFiles_Action ) {
        if ( downloadIsCompleted ) {
            downloadIsCompleted = false;
            copyFiles_Action->setToolTip(
                        "Cancel Copying Files to Guest");
            copyFiles_Action->setIcon(
                        QIcon::fromTheme("delete"));
            parameters.method = Methods::copyFilesToVirtDomain;
        } else {
            parameters.method = Methods::cancelCopyFilesToVirtDomain;
        };
    } else if ( action == copyToClipboard ) {
        parameters.method = Methods::copyToClipboardFromVirtDomain;
    } else if ( action == pasteClipboard ) {
        parameters.method = Methods::pasteClipboardToVirtDomain;
    } else if ( action == fullScreen ) {
        parameters.method = Methods::fullScreenVirtDomain;
    } else if ( action == scaled_to ) {
        parameters.method = Methods::scaleScreenVirtDomain;
    } else return;
    emit execMethod(parameters);
}
void ViewerToolBar::moveEvent(QMoveEvent *ev)
{
    emit positionChanged(ev->pos());
}
