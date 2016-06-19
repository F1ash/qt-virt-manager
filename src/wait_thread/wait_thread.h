#ifndef WAIT_THREAD_H
#define WAIT_THREAD_H

#include <QThread>
#include "virt_objects/virt_connect/connection_list/conn_list_widget.h"

class Wait : public QThread
{
    Q_OBJECT
public:
    explicit Wait(
            QObject        *parent  = nullptr,
            ConnectionList *wdgList = nullptr);

private:
    ConnectionList     *wdg;

private slots:
    void                run();
};
#endif   // WAIT_THREAD_H
