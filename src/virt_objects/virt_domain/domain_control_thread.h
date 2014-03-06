#ifndef DOM_CONTROL_THREAD_H
#define DOM_CONTROL_THREAD_H

#include <QThread>
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
    DESTROY_DOMAIN,
    UNDEFINE_DOMAIN,
    CHANGE_DOM_AUTOSTART,
    GET_DOM_XML_DESC,
    DOM_EMPTY_ACTION
};

class DomControlThread : public QThread
{
    Q_OBJECT
public:
    explicit DomControlThread(QObject *parent = 0);

signals:
    void errorMsg(QString);
    void resultData(DomActions, QStringList);

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
    QStringList getAllDomainList();
    QStringList createDomain();
    QStringList defineDomain();
    QStringList startDomain();
    QStringList destroyDomain();
    QStringList undefineDomain();
    QStringList changeAutoStartDomain();
    QStringList getVirtNetXMLDesc();

    void sendConnErrors();
    void sendGlobalErrors();

};

#endif // DOM_CONTROL_THREAD_H
