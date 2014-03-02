#ifndef CONN_ALIVE_THREAD_H
#define CONN_ALIVE_THREAD_H

#include <QThread>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include <QDebug>

class ConnAliveThread : public QThread
{
    Q_OBJECT
public:
    explicit ConnAliveThread(QObject *parent = 0);
    ~ConnAliveThread();

signals:
    void connMsg(QString);

private:
    bool            keep_alive;
    QString         URI;

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

};

#endif // CONN_ALIVE_THREAD_H
