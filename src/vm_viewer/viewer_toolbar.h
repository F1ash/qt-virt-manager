#ifndef VIEWER_TOOLBAR_H
#define VIEWER_TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QMenu>
#include <QTimerEvent>
#include <QMoveEvent>
#include <QDebug>

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

signals:
    void             execMethod(const QStringList&);
    void             positionChanged(const QPoint&);

private slots:
    void             showMenu();
    void             detectTriggerredAction(QAction*);
    void             moveEvent(QMoveEvent*);

public slots:
    void             changeCopypasteState(bool);
};

#endif // VIEWER_TOOLBAR_H
