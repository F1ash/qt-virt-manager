#ifndef DOMAIN_MONITOR_THREAD_H
#define DOMAIN_MONITOR_THREAD_H

#include <QThread>
#if (QT_VERSION >= QT_VERSION_CHECK(4, 7, 0))
#include <QElapsedTimer>
#else
#include <QTime>
#endif
#include "virt_objects/_virt_thread.h"
//#include <QDebug>

class DomainMonitorThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit DomainMonitorThread(
            QObject        *parent      = Q_NULLPTR,
            virConnectPtr*  connPtrPtr  = Q_NULLPTR,
            QString         _domainName = QString());
    ~DomainMonitorThread();

signals:
    void                 dataChanged(int, int, int, int);

private:
    virDomainPtr         domain;
    const QString        domainName;
    quint64              prev_cpuTime;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QElapsedTimer        tMark;
#else
    QTime                tMark;
#endif
    bool                 firstStep;

public slots:

private slots:
    void                 run();
};

#endif // DOMAIN_MONITOR_THREAD_H
