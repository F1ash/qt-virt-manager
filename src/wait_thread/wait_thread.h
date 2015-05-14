#ifndef WAIT_THREAD_H
#define WAIT_THREAD_H

#include <QThread>
#include "layout/conn_list_widget.h"

class Wait : public QThread
{
    Q_OBJECT
public:
    Wait(QObject *parent = NULL,
         ConnectList *wdgList = NULL);
    ~Wait();

private:
    ConnectList     *wdg;

private slots:
    void             run();
};
#endif   // WAIT_THREAD_H
