#ifndef CONTROL_THREAD_H
#define CONTROL_THREAD_H

#include <QThread>
#include <QDir>
#include <QTemporaryFile>
#include <QStringList>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include <QDebug>

enum Actions {
    GET_ALL_ENTITY,
    CREATE_ENTITY,
    DEFINE_ENTITY,
    EDIT_ENTITY,
    START_ENTITY,
    PAUSE_ENTITY,
    DESTROY_ENTITY,
    RESET_ENTITY,
    REBOOT_ENTITY,
    SHUTDOWN_ENTITY,
    SAVE_ENTITY,
    RESTORE_ENTITY,
    UNDEFINE_ENTITY,
    DELETE_ENTITY,
    MIGRATE_ENTITY,
    DOWNLOAD_ENTITY,
    RESIZE_ENTITY,
    UPLOAD_ENTITY,
    WIPE_ENTITY,
    CHANGE_ENTITY_AUTOSTART,
    GET_XML_DESCRIPTION,
    _EMPTY_ACTION
};

struct Result {
    QString         name   = QString();
    uint            number = 0;
    QString         type   = QString();
    int             action = -1;
    bool            result = false;
    QStringList     msg    = QStringList();
};

class ControlThread : public QThread
{
    Q_OBJECT
public:
    explicit ControlThread(QObject *parent = NULL);
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
