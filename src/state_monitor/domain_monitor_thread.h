#ifndef DOMAIN_MONITOR_THREAD_H
#define DOMAIN_MONITOR_THREAD_H

#include <QThread>
#include <QTime>
#include "virt_objects/_virt_thread.h"
//#include <QDebug>

class DomainMonitorThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit DomainMonitorThread(
            QObject        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr,
            QString         _domainName = QString());
    ~DomainMonitorThread();

signals:
    void                 dataChanged(int, int, int, int);

private:
    virDomainPtr         domain;
    const QString        domainName;
    quint64              prev_cpuTime;
    QTime                tMark;
    bool                 firstStep;

public slots:

private slots:
    void                 run();
};

#endif // DOMAIN_MONITOR_THREAD_H
