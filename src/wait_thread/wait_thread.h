#ifndef WAIT_THREAD_H
#define WAIT_THREAD_H

#include <QThread>
#include "layout/conn_list_widget.h"
#include "vm_viewer/vm_viewer.h"

typedef QMap<QString, VM_Viewer*> ViewerMap;

class Wait : public QThread
{
    Q_OBJECT
public:
    Wait(QObject *parent = 0, ConnectList *wdgList = NULL,
         ViewerMap map = ViewerMap());
    ~Wait();

signals:
    void refreshProcessingState();

private:
    ConnectList     *wdg;
    ViewerMap        vm_displayed_map;
    bool             processingState;

public slots:
    void setProcessingState(bool);

private slots:
    void run();
};
#endif   // WAIT_THREAD_H
