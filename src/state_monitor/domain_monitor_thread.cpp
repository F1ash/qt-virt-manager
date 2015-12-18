#include "domain_monitor_thread.h"

DomainMonitorThread::DomainMonitorThread(
        QObject *parent, virConnectPtr* connPtrPtr, QString _domainName) :
    _VirtThread(parent, connPtrPtr), domainName(_domainName)
{
    prev_cpuTime = 0;
    firstStep = true;
    tMark.start();
}
DomainMonitorThread::~DomainMonitorThread()
{
    // release the reference because no longer required
    if ( ptr_ConnPtr!=NULL ) {
        if ( NULL!=domain ) {
            if ( virDomainFree(domain)<0 )
                sendConnErrors();
        };
        int ret = virConnectClose(*ptr_ConnPtr);
        //qDebug()<<"virConnectRef -1"<<"DomainStateViewer"<<domainName<<(ret+1>0);
        // for reject the multiple releasing the reference
        if ( ret<0 ) sendConnErrors();
        ptr_ConnPtr = NULL;
    };
    wait(30000);
}

/* public slots */

/* private slots */
void DomainMonitorThread::run()
{
    if ( firstStep ) {
        // for new virConnect usage create the new virConnectRef[erence]
        if ( NULL!=ptr_ConnPtr && NULL!=*ptr_ConnPtr ) {
            if ( virConnectRef(*ptr_ConnPtr)<0 ) {
                ptr_ConnPtr = NULL;
                sendConnErrors();
            };
            //qDebug()<<"virConnectRef +1"<<"DomainMonitorThread"<<domainName<<(ret+1>0);
            domain = virDomainLookupByName(
                        *ptr_ConnPtr, domainName.toUtf8().data());
            if ( NULL==domain ) sendConnErrors();
        };
    };
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
