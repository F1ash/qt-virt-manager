#ifndef CONN_ALIVE_THREAD_H
#define CONN_ALIVE_THREAD_H

#include <QThread>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include <QDebug>

enum CONN_STATE {
    FAILED  = -1,
    STOPPED,
    RUNNING
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

private:
    bool            keep_alive;
    QString         URI = QString();

    virConnectPtr   conn = NULL;
    virErrorPtr     virtErrors;
    bool            registered;

public slots:
    void            setData(QString&);
    void            setKeepAlive(bool);
    bool            getKeepAlive() const;
    virConnect*     getConnect() const;

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
    void closeConnect(int);

};

#endif // CONN_ALIVE_THREAD_H
