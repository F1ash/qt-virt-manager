#ifndef DOM_CONTROL_THREAD_H
#define DOM_CONTROL_THREAD_H

#include "virt_objects/control_thread.h"

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

class DomControlThread : public ControlThread
{
    Q_OBJECT
public:
    explicit DomControlThread(QObject *parent = 0);

signals:
    void resultData(DomActions, Result);

private:
    DomActions  action;

public slots:
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

};

#endif // DOM_CONTROL_THREAD_H
