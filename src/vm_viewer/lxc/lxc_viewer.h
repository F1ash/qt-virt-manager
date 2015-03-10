#ifndef LXC_VIEWER_H
#define LXC_VIEWER_H

#include <QTimerEvent>
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

signals:
    void            termEOF();

private:
    virStream      *stream = NULL;
    int             ptySlaveFd = -1;

public slots:

private slots:
    void            timerEvent(QTimerEvent*);
    void            setTerminalParameters();
    void            closeEvent(QCloseEvent*);
    int             registerStreamEvents();
    int             unregisterStreamEvents();
    static void     freeData(void*);
    static void     streamEventCallBack(virStreamPtr, int, void*);
    void            updateStreamEvents(virStreamPtr, int);
    void            sendDataToDisplay(virStreamPtr);
    void            sendDataToVMachine(const char*, int);
    void            closeStream();
};

#endif // LXC_VIEWER_H
