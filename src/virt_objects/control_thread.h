#ifndef CONTROL_THREAD_H
#define CONTROL_THREAD_H

#include "_virt_thread.h"
#include "virt_entity_config.h"
#include <QDir>
#include <QTemporaryFile>
#include <QStringList>

class ControlThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit ControlThread(QObject *parent = Q_NULLPTR);
    TASK             task;
    QString          currConnName;

signals:
    void             resultData(Result);

public slots:
    bool             setCurrentWorkConnection(
                        virConnectPtr*,
                        uint i=0,
                        QString _name="");

    virtual void     execAction(uint, TASK);
};

#endif // CONTROL_THREAD_H
