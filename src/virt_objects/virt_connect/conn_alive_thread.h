#ifndef CONN_ALIVE_THREAD_H
#define CONN_ALIVE_THREAD_H

#include "virt_objects/_virt_thread.h"

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
    void            poolStateChanged();
    void            secStateChanged();
    void            connClosed(bool);
    void            domainEnd(const QString&);

private:
    bool            onView;
    int             domainsLifeCycleCallback;
    int             networksLifeCycleCallback;
#if LIBVIR_VERSION_NUMBER >= 2000000
    int             poolsLifeCycleCallback;
#endif
#if LIBVIR_VERSION_NUMBER >= 3000000
    int             secretsLifeCycleCallback;
#endif
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
#if LIBVIR_VERSION_NUMBER >= 2000000
    static  int     poolEventCallback(virConnectPtr, virStoragePoolPtr,
                                     int, int, void*);
#endif
#if LIBVIR_VERSION_NUMBER >= 3000000
    static  int     secEventCallback(virConnectPtr, virSecretPtr,
                                     int, int, void*);
#endif
    const char*     domEventToString(int event);
    const char*     domEventDetailToString(int event, int detail, bool *end);
    const char*     netEventToString(int event);
    const char*     netEventDetailToString(int event, int detail);
#if LIBVIR_VERSION_NUMBER >= 2000000
    const char*     poolEventToString(int event);
    const char*     poolEventDetailToString(int event, int detail);
#endif
#if LIBVIR_VERSION_NUMBER >= 3000000
    const char*     secEventToString(int event);
    const char*     secEventDetailToString(int event, int detail);
#endif
    void            closeConnection(int);
    void            getAuthCredentials(const QString&);
};

#endif // CONN_ALIVE_THREAD_H
