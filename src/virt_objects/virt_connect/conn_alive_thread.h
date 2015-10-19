#ifndef CONN_ALIVE_THREAD_H
#define CONN_ALIVE_THREAD_H

#include "virt_objects/virt_entity_config.h"
#include "virt_objects/_virt_thread.h"
#include <QTime>

#define WAIT_AUTH 300    // dev 10 (sec.)

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

/* The list of credential types supported by our auth callback */
static int credTypes[] = {
    VIR_CRED_AUTHNAME,
    VIR_CRED_PASSPHRASE
};

/* The auth struct that will be passed to virConnectOpenAuth */
static virConnectAuth auth = {
    credTypes,
    sizeof(credTypes) / sizeof(int),
    NULL, // cb will be initialized in thread
    NULL, // cbdata will be initialized in thread
};

class ConnAliveThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit ConnAliveThread(QObject *parent = NULL);
    bool            onView;

signals:
    void            connMsg(const QString&);
    void            changeConnState(CONN_STATE);
    void            authRequested(QString&);
    void            domStateChanged(Result);
    void            netStateChanged(Result);
    void            connClosed(bool);
    void            domainEnd(QString&);

private:
    int             domainsLifeCycleCallback;
    int             networkLifeCycleCallback;
    bool            authWaitKey;
    AuthData        authData;
    QString         URI;
    bool            closeCallbackRegistered;
    virConnectPtr   _connPtr;

public slots:
    void            setData(QString&);
    void            closeConnection();
    virConnectPtr*  getPtr_connectionPtr();
    void            setAuthCredentials(QString&, QString&);

private slots:
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
    void            getAuthCredentials(QString&);
};

#endif // CONN_ALIVE_THREAD_H
