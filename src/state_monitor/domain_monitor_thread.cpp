#include "domain_monitor_thread.h"

DomainMonitorThread::DomainMonitorThread(
        QObject *parent, virConnectPtr conn, QString _domainName) :
    QThread(parent), currWorkConn(conn), domainName(_domainName)
{
}
