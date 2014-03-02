#ifndef WAIT_THREAD_H
#define WAIT_THREAD_H

#include <QThread>
#include "layout/conn_list_widget.h"

class Wait : public QThread
{
    Q_OBJECT
public:
    Wait(QObject *parent);
    ~Wait();

signals:
    void refreshProcessingState();

private:
    ConnectList  *wdg;
    bool          processingState;

public slots:
    void setPtr(ConnectList*);
    void setProcessingState(bool);

private slots:
    void run();
};
#endif   // WAIT_THREAD_H
