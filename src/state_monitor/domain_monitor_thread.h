#ifndef DOMAIN_MONITOR_THREAD_H
#define DOMAIN_MONITOR_THREAD_H

#include <QThread>
#include <QTime>
#include <QDebug>
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>

class DomainMonitorThread : public QThread
{
    Q_OBJECT
public:
    explicit DomainMonitorThread(
            QObject        *parent  = NULL,
            virConnectPtr*  connPtr = NULL,
            QString         _domainName = QString());
    ~DomainMonitorThread();

signals:
    void                 dataChanged(int, int, int, int);

private:
    virConnectPtr*       ptr_ConnPtr;
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
