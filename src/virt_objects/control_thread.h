#ifndef CONTROL_THREAD_H
#define CONTROL_THREAD_H

#include <QThread>
#include <QDir>
#include <QTemporaryFile>
#include <QStringList>
#include <QDebug>
#include "virt_entity_config.h"

class ControlThread : public QThread
{
    Q_OBJECT
public:
    explicit ControlThread(QObject *parent = NULL);
    virtual ~ControlThread();
    QStringList      args;
    bool             keep_alive = false;
    uint             number;
    Actions          action;
    QString          currConnName;
    virConnect      *currWorkConnect = NULL;
    virErrorPtr      virtErrors;

signals:
    void             errorMsg(QString);
    void             resultData(Result);

public slots:
    bool             setCurrentWorkConnect(
                        virConnectPtr,
                        uint i=0,
                        QString _name="");
    void             stop();

    void             sendConnErrors();
    void             sendGlobalErrors();

    virtual void     execAction(Actions, QStringList);
    virtual void     run();
};

#endif // CONTROL_THREAD_H
