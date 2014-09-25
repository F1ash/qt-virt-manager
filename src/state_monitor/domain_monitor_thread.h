#ifndef DOMAIN_MONITOR_THREAD_H
#define DOMAIN_MONITOR_THREAD_H

#include <QThread>
#include <QDebug>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"

class DomainMonitorThread : public QThread
{
    Q_OBJECT
public:
    explicit DomainMonitorThread(
            QObject *parent = 0,
            virConnectPtr conn = NULL,
            QString _domainName = QString());

signals:

private:
    virConnectPtr        currWorkConn;
    const QString        domainName;

public slots:

};

#endif // DOMAIN_MONITOR_THREAD_H
