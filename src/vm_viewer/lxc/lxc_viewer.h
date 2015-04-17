#ifndef LXC_VIEWER_H
#define LXC_VIEWER_H

#include <QTimerEvent>
#include "vm_viewer/qterminal/mainwindow.h"
#include "lxc_viewer_thread.h"
#include <QDebug>

class LXC_Viewer : public TermMainWindow
{
    Q_OBJECT
public:
    explicit LXC_Viewer(
            QWidget         *parent = NULL,
            virConnect      *conn = NULL,
            QString          arg1 = QString(),
            QString          arg2 = QString(),
            const QString&   work_dir = NULL,
            const QString&   command = NULL);
    ~LXC_Viewer();

signals:

private:
    LXC_ViewerThread    *viewerThread = NULL;

public slots:

private slots:
    void                 timerEvent(QTimerEvent*);
    void                 setTerminalParameters();
    void                 closeEvent(QCloseEvent*);
};

#endif // LXC_VIEWER_H
