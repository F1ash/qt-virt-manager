#ifndef CONN_ALIVE_THREAD_H
#define CONN_ALIVE_THREAD_H

#include "virt_objects/_virt_thread.h"
#include <QTime>

enum CONN_STATE {
    FAILED  = -1,
    CLOSED,
    RUNNING,
    CONNECT
};

/* Struct to pass the credentials to the auth callback via the cbdata pointer */
struct _AuthData {
    char *username;
    char *password;
};

typedef struct _AuthData AuthData;

class ConnAliveThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit ConnAliveThread(QObject *parent = nullptr);
    ~ConnAliveThread();
    void            setOnViewState(bool);
    bool            getOnViewState() const;
    virConnectPtr*  getPtr_connectionPtr() const;
    void            setData(const QString&);
    void            closeConnection();
    void            setAuthCredentials(const QString&, const QString&);

signals:
    void            connMsg(const QString&);
    void            changeConnState(CONN_STATE);
    void            authRequested(const QString&);
    void            domStateChanged();
    void            netStateChanged();
    void            connClosed(bool);
    void            domainEnd(const QString&);

private:
    bool            onView;
    int             domainsLifeCycleCallback;
    int             networkLifeCycleCallback;
    bool            authWaitKey;
    AuthData        authData;
    QString         URI;
    bool            closeCallbackRegistered;
    virConnectPtr   _connPtr;

    void            run();
    void            openConnection();
    void            registerConnEvents();
    void            unregisterConnEvents();
    static void     freeData(void*);
    static void     connEventCallBack(virConnectPtr, int, void*);
    static  int     authCallback(virConnectCredentialPtr, unsigned int, void*);
    static  int     domEventCallback(virConnectPtr, virDomainPtr,
                                     int, int, void*);
    static  int     netEventCallback(virConnectPtr, virNetworkPtr,
                                     int, int, void*);
    const char*     domEventToString(int event);
    const char*     domEventDetailToString(int event, int detail, bool *end);
    const char*     netEventToString(int event);
    const char*     netEventDetailToString(int event, int detail);
    void            closeConnection(int);
    void            getAuthCredentials(const QString&);
};

#endif // CONN_ALIVE_THREAD_H
