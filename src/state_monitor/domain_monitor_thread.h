#ifndef DOMAIN_MONITOR_THREAD_H
#define DOMAIN_MONITOR_THREAD_H

#include <QThread>
#include "time.h"
#include <QDebug>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"

class DomainMonitorThread : public QThread
{
    Q_OBJECT
public:
    explicit DomainMonitorThread(
            QObject *parent = NULL,
            virConnectPtr conn = NULL,
            QString _domainName = QString());

signals:
    void dataChanged(int, int, int, int);

private:
    virConnectPtr        currWorkConn;
    virDomainPtr         domain;
    const QString        domainName;
    uint                 prev_cpuTime = 0;
    uint                 prev_time_mark = 0;
    uint                 nr_cores = 0;

public slots:
    void                 stop();

private slots:
    void                 run();
};

#endif // DOMAIN_MONITOR_THREAD_H
