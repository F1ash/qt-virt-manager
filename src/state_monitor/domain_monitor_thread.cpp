#include "domain_monitor_thread.h"

DomainMonitorThread::DomainMonitorThread(
        QObject *parent, virConnectPtr conn, QString _domainName) :
    QThread(parent), currWorkConn(conn), domainName(_domainName)
{
    // for new virConnect usage create the new virConnectRef[erence]
    int ret = virConnectRef(currWorkConn);
    if ( ret<0 ) currWorkConn = NULL;
    //qDebug()<<"virConnectRef +1"<<"DomainMonitorThread"<<domainName<<(ret+1>0);
    domain = virDomainLookupByName(
                currWorkConn, domainName.toUtf8().data());
    domainsBalloonChangeCallback = virConnectDomainEventRegisterAny(
                currWorkConn,
                domain,
                VIR_DOMAIN_EVENT_ID_BALLOON_CHANGE,
    // set domainsBalloonChangeCallback signature
                VIR_DOMAIN_EVENT_CALLBACK(domEventCallback),
                this,
    // don't register freeData, because it remove this thread
                NULL);
    prev_cpuTime = 0;
    firstStep = true;
    tMark.start();
    qDebug()<<domainsBalloonChangeCallback<<"BalloonChange";
}
DomainMonitorThread::~DomainMonitorThread()
{
    // release the reference because no longer required
    if ( currWorkConn!=NULL ) {
        virConnectDomainEventDeregisterAny(
                    currWorkConn, domainsBalloonChangeCallback);
        int ret = virConnectClose(currWorkConn);
        //qDebug()<<"virConnectRef -1"<<"DomainStateViewer"<<domainName<<(ret+1>0);
        // for reject the multiple releasing the reference
        currWorkConn = NULL;
    };
    wait(30000);
}

/* public slots */
void DomainMonitorThread::stop()
{
    if ( NULL!=domain ) {
        virDomainFree(domain);
    }
}

/* private slots */
void DomainMonitorThread::run()
{
    if ( NULL!=domain ) {
        virDomainInfo info;
        quint64 curr_cpuTime, cpu_time_diff, _time_diff,
                CPU_percent, MEM_percent, MEM;
        if ( virDomainGetInfo(domain, &info)+1 ) {
            curr_cpuTime = info.cpuTime;
            _time_diff = tMark.elapsed()/1000;
            cpu_time_diff = (firstStep)? 0 : curr_cpuTime - prev_cpuTime;
            CPU_percent = (qreal)100/(_time_diff * info.nrVirtCpu)*(qreal)cpu_time_diff/1000000000;
            tMark.restart();
            MEM = info.memory;
            MEM_percent = 100*(qreal)MEM / info.maxMem;
            prev_cpuTime = curr_cpuTime;
            if ( firstStep ) firstStep = false;
        };
        if ( info.state == VIR_DOMAIN_RUNNING || info.state == VIR_DOMAIN_PAUSED ) {
            emit dataChanged(info.state, CPU_percent, MEM_percent, MEM);
        } else
            emit dataChanged(info.state, 0, 0, 0);
    } else {
        firstStep = true;
    };
}

int  DomainMonitorThread::domEventCallback(virConnectPtr _conn, virDomainPtr dom, qulonglong actual, void *opaque)
{
    qDebug()<<"domEventCallback"<<_conn;
    DomainMonitorThread *obj = static_cast<DomainMonitorThread*>(opaque);
    if ( NULL==obj || obj->currWorkConn!=_conn ) return 0;
    QString msg;
    msg = QString("<b>'%1'</b> Domain %2")
           .arg(virDomainGetName(dom))
           .arg(QString::number(actual));
    qDebug()<<msg<<"BalloonChange";
    return 0;
}
