#ifndef DOM_CONTROL_THREAD_H
#define DOM_CONTROL_THREAD_H

#include <QThread>
#include <QDir>
#include <QTemporaryFile>
#include <QStringList>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include <QDebug>

enum DomActions {
    GET_ALL_DOMAIN,
    CREATE_DOMAIN,
    DEFINE_DOMAIN,
    START_DOMAIN,
    PAUSE_DOMAIN,
    DESTROY_DOMAIN,
    RESET_DOMAIN,
    REBOOT_DOMAIN,
    SHUTDOWN_DOMAIN,
    SAVE_DOMAIN,
    RESTORE_DOMAIN,
    UNDEFINE_DOMAIN,
    CHANGE_DOM_AUTOSTART,
    GET_DOM_XML_DESC,
    DOM_EMPTY_ACTION
};

struct Result {
    QString         name   = QString();
    bool            result = false;
    QStringList     msg    = QStringList();
};

class DomControlThread : public QThread
{
    Q_OBJECT
public:
    explicit DomControlThread(QObject *parent = 0);

signals:
    void errorMsg(QString);
    void resultData(DomActions, Result);

private:
    DomActions       action;
    QStringList      args;
    bool             keep_alive;
    virConnect      *currWorkConnect = NULL;
    virErrorPtr      virtErrors;

public slots:
    bool setCurrentWorkConnect(virConnectPtr);
    void stop();
    void execAction(DomActions, QStringList);

private slots:
    void run();
    Result getAllDomainList();
    Result createDomain();
    Result defineDomain();
    Result startDomain();
    Result pauseDomain();
    Result destroyDomain();
    Result resetDomain();
    Result rebootDomain();
    Result shutdownDomain();
    Result saveDomain();
    Result restoreDomain();
    Result undefineDomain();
    Result changeAutoStartDomain();
    Result getDomainXMLDesc();

    void sendConnErrors();
    void sendGlobalErrors();

};

#endif // DOM_CONTROL_THREAD_H
