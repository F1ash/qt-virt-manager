#ifndef CONN_ALIVE_THREAD_H
#define CONN_ALIVE_THREAD_H

#include <QThread>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include <QDebug>

#define WAIT_AUTH 300    // dev 10 (sec.)

enum CONN_STATE {
    FAILED  = -1,
    STOPPED,
    RUNNING
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

class ConnAliveThread : public QThread
{
    Q_OBJECT
public:
    explicit ConnAliveThread(QObject *parent = 0);
    ~ConnAliveThread();

signals:
    void connMsg(const QString&);
    void changeConnState(CONN_STATE);
    void connectClosed(int);
    void authRequested(QString&);

private:
    bool            keep_alive;
    bool            authWaitKey;
    AuthData        authData;
    QString         URI = QString();

    virConnectPtr   conn = NULL;
    virErrorPtr     virtErrors;
    bool            registered;

public slots:
    void            setData(QString&);
    void            setKeepAlive(bool);
    bool            getKeepAlive() const;
    virConnect*     getConnect() const;
    void            setAuthCredentials(QString&, QString&);

private slots:
    void run();
    void openConnect();
    void closeConnect();
    void sendConnErrors();
    void sendGlobalErrors();
    void registerConnEvents();
    void unregisterConnEvents();
    static void     freeData(void*);
    static void     connEventCallBack(virConnectPtr, int, void*);
    static  int     authCallback(virConnectCredentialPtr, unsigned int, void*);
    void closeConnect(int);
    void getAuthCredentials(QString&);

};

#endif // CONN_ALIVE_THREAD_H
