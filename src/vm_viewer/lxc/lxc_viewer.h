#ifndef LXC_VIEWER_H
#define LXC_VIEWER_H

#include <QTimerEvent>
#include <QSocketNotifier>
#include <QProgressBar>
#include <QStatusBar>
#include "vm_viewer/qterminal/mainwindow.h"
#include <unistd.h>
#include <QDebug>

class LXC_Viewer : public TermMainWindow
{
    Q_OBJECT
public:
    explicit LXC_Viewer(QWidget *parent = NULL,
            virConnect *conn = NULL,
            QString arg1 = QString(),
            QString arg2 = QString(),
            const QString& work_dir = NULL,
            const QString& command = NULL);
    ~LXC_Viewer();

private:
    virDomain      *domainPtr = NULL;
    virStream      *stream = NULL;
    int             ptySlaveFd = -1;
    size_t          buffDiff = 0;
    uint            timerId = 0;
    uint            killTimerId = 0;
    uint            counter = 0;
    QSocketNotifier
                   *readSlaveFd = NULL;
    QProgressBar   *closeProcess = NULL;

public slots:
    void            closeTerminal();

private slots:
    void            timerEvent(QTimerEvent*);
    void            setTerminalParameters();
    void            closeEvent(QCloseEvent*);
    virDomain*      getDomainPtr() const;
    int             registerStreamEvents();
    int             unregisterStreamEvents();
    static void     freeData(void*);
    static void     streamEventCallBack(virStreamPtr, int, void*);
    void            updateStreamEvents(virStreamPtr, int);
    void            sendDataToDisplay(virStreamPtr);
    void            sendDataToVMachine(int);
    void            closeStream();
};

#endif // LXC_VIEWER_H
