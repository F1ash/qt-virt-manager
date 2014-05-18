#include "domain_control_thread.h"

DomControlThread::DomControlThread(QObject *parent) :
    QThread(parent)
{
    qRegisterMetaType<DomActions>("DomActions");
    qRegisterMetaType<Result>("Result");
}

/* public slots */
bool DomControlThread::setCurrentWorkConnect(virConnectPtr conn)
{
    keep_alive = true;
    currWorkConnect = conn;
    //qDebug()<<"net_thread"<<currWorkConnect;
}
void DomControlThread::stop() { keep_alive = false; }
void DomControlThread::execAction(DomActions act, QStringList _args)
{
    if ( keep_alive && !isRunning() ) {
        action = act;
        args = _args;
        start();
    };
}

/* private slots */
void DomControlThread::run()
{
    Result result;
    switch (action) {
    case GET_ALL_DOMAIN :
        result = getAllDomainList();
        break;
    case CREATE_DOMAIN :
        result = createDomain();
        break;
    case DEFINE_DOMAIN :
        result = defineDomain();
        break;
    case START_DOMAIN :
        result = startDomain();
        break;
    case PAUSE_DOMAIN :
        result = pauseDomain();
        break;
    case DESTROY_DOMAIN :
        result = destroyDomain();
        break;
    case RESET_DOMAIN :
        result = resetDomain();
        break;
    case REBOOT_DOMAIN :
        result = rebootDomain();
        break;
    case SHUTDOWN_DOMAIN :
        result = shutdownDomain();
        break;
    case SAVE_DOMAIN :
        result = saveDomain();
        break;
    case RESTORE_DOMAIN :
        result = restoreDomain();
        break;
    case UNDEFINE_DOMAIN :
        result = undefineDomain();
        break;
    case CHANGE_DOM_AUTOSTART :
        result = changeAutoStartDomain();
        break;
    case GET_DOM_XML_DESC :
        result = getDomainXMLDesc();
        break;
    default:
        break;
    };
    emit resultData(action, result);
}
Result DomControlThread::getAllDomainList()
{
    Result result;
    QStringList domainList;
    if ( currWorkConnect!=NULL && keep_alive ) {
        virDomainPtr *domain;
        unsigned int flags = VIR_CONNECT_LIST_DOMAINS_ACTIVE |
                             VIR_CONNECT_LIST_DOMAINS_INACTIVE;
        int ret = virConnectListAllDomains( currWorkConnect, &domain, flags);
        if ( ret<0 ) {
            sendConnErrors();
            free(domain);
            return result;
        };

        int i = 0;
        while ( domain[i] != NULL ) {
            QStringList currentAttr;
            QString autostartStr;
            int is_autostart = 0;
            if (virDomainGetAutostart(domain[i], &is_autostart) < 0) {
                autostartStr.append("no autostart");
            } else autostartStr.append( is_autostart ? "yes" : "no" );
            int state;
            int reason;
            // flags : extra flags; not used yet, so callers should always pass 0
            flags = 0;
            QString domainState;
            if ( virDomainGetState(domain[i], &state, &reason, flags)+1 ) {
                switch (state) {
                case VIR_DOMAIN_NOSTATE:
                    domainState.append("NOSTATE");
                    break;
                case VIR_DOMAIN_RUNNING:
                    domainState.append("RUNNING");
                    break;
                case VIR_DOMAIN_BLOCKED:
                    domainState.append("BLOCKED");
                    break;
                case VIR_DOMAIN_PAUSED:
                    domainState.append("PAUSED");
                    break;
                case VIR_DOMAIN_SHUTDOWN:
                    domainState.append("SHUTDOWN");
                    break;
                case VIR_DOMAIN_SHUTOFF:
                    domainState.append("SHUTOFF");
                    break;
                case VIR_DOMAIN_CRASHED:
                    domainState.append("CRASHED");
                    break;
                case VIR_DOMAIN_PMSUSPENDED:
                    domainState.append("PMSUSPENDED");
                    break;
                default:
                    break;
                }
            } else domainState.append("ERROR");
            currentAttr<< QString().fromUtf8( virDomainGetName(domain[i]) )
                       << QString("%1:%2").arg( virDomainIsActive(domain[i]) ? "active" : "inactive" ).arg(domainState)
                       << autostartStr
                       << QString( virDomainIsPersistent(domain[i]) ? "yes" : "no" );
            domainList.append(currentAttr.join(" "));
            //qDebug()<<currentAttr;
            /*
            virDomainInfo info;
            if ( virDomainGetInfo(domain[i], &info)+1 ) {
                qDebug()<<info.state
                        <<info.maxMem
                        <<info.memory
                        <<info.nrVirtCpu
                        <<info.cpuTime;
            };
            virDomainFree(domain[i]);
            */
            i++;
        };
        free(domain);
    };
    /*
    if ( currWorkConnect!=NULL && keep_alive ) {
        virNodeDevicePtr *nodeDevice;
        unsigned int flags = VIR_CONNECT_LIST_NODE_DEVICES_CAP_SYSTEM | VIR_CONNECT_LIST_NODE_DEVICES_CAP_PCI_DEV |
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_USB_DEV | VIR_CONNECT_LIST_NODE_DEVICES_CAP_USB_INTERFACE |
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_NET | VIR_CONNECT_LIST_NODE_DEVICES_CAP_SCSI_HOST |
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_SCSI_TARGET | VIR_CONNECT_LIST_NODE_DEVICES_CAP_SCSI |
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_STORAGE | VIR_CONNECT_LIST_NODE_DEVICES_CAP_FC_HOST |
                VIR_CONNECT_LIST_NODE_DEVICES_CAP_VPORTS | VIR_CONNECT_LIST_NODE_DEVICES_CAP_SCSI_GENERIC;
        int ret = virConnectListAllNodeDevices( currWorkConnect, &nodeDevice, flags);
        if ( ret<0 ) {
            sendConnErrors();
            free(nodeDevice);
        } else {

        int i = 0;
        while ( nodeDevice[i] != NULL ) {
            QStringList currentAttr;
            currentAttr<< QString( virNodeDeviceGetName(nodeDevice[i]) );
            qDebug()<<currentAttr;
            virNodeDeviceFree(nodeDevice[i]);
            i++;
        };
        free(nodeDevice);
        };
    };
    */
    result.result = true;
    result.msg = domainList;
    return result;
}
Result DomControlThread::createDomain()
{
    Result result;
    QString path = args.first();
    QByteArray xmlData;
    QFile f;
    f.setFileName(path);
    if ( !f.open(QIODevice::ReadOnly) ) {
        emit errorMsg( QString("File \"%1\"\nnot opened.").arg(path) );
        return result;
    };
    xmlData = f.readAll();
    f.close();
    virDomainPtr domain = virDomainCreateXML(currWorkConnect, xmlData.data(), VIR_DOMAIN_START_AUTODESTROY);
    if ( domain==NULL ) {
        sendConnErrors();
        return result;
    };
    result.name = QString().fromUtf8( virDomainGetName(domain) );
    result.result = true;
    result.msg.append(QString("'%1' Domain from\n\"%2\"\nis created.")
                     .arg(result.name).arg(path));
    virDomainFree(domain);
    return result;
}
Result DomControlThread::defineDomain()
{
    Result result;
    QString path = args.first();
    QByteArray xmlData;
    QFile f;
    f.setFileName(path);
    if ( !f.open(QIODevice::ReadOnly) ) {
        emit errorMsg( QString("File \"%1\"\nnot opened.").arg(path) );
        return result;
    };
    xmlData = f.readAll();
    f.close();
    virDomainPtr domain = virDomainDefineXML(currWorkConnect, xmlData.data());
    if ( domain==NULL ) {
        sendConnErrors();
        return result;
    };
    result.name = QString().fromUtf8( virDomainGetName(domain) );
    result.result = true;
    result.msg.append(QString("'%1' Domain from\n\"%2\"\nis defined.")
                  .arg(result.name).arg(path));
    virDomainFree(domain);
    return result;
}
Result DomControlThread::startDomain()
{
    Result result;
    QString name = args.first();

    bool started = false;
    virDomainPtr domain = virDomainLookupByName(currWorkConnect, name.toUtf8().data());

    if ( domain!=NULL ) {
        started = (virDomainCreate(domain)+1) ? true : false;
        if (!started) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.name = name;
    result.result = started;
    result.msg.append(QString("'%1' Domain %2 Started.").arg(name).arg((started)?"":"don't"));
    return result;
}
Result DomControlThread::pauseDomain()
{
    Result result;
    QString name = args.first();
    QString state = args.last();
    bool invoked = false;

    virDomainPtr domain = virDomainLookupByName(currWorkConnect, name.toUtf8().data());

    if ( domain!=NULL ) {
        if ( state=="RUNNING" ) {
            invoked = (virDomainSuspend(domain)+1) ? true : false;
            if (!invoked) sendConnErrors();
        } else if ( state=="PAUSED" ) {
            invoked = (virDomainResume(domain)+1) ? true : false;
            if (!invoked) sendConnErrors();
        } else 0;
        virDomainFree(domain);
    } else sendConnErrors();
    result.name = name;
    result.result = invoked;
    result.msg.append(QString("'%1' Domain state %2 changed.").arg(name).arg((invoked)?"":"don't"));
    return result;
}
Result DomControlThread::destroyDomain()
{
    Result result;
    QString name = args.first();

    bool deleted = false;
    virDomainPtr domain = virDomainLookupByName(currWorkConnect, name.toUtf8().data());
    if ( domain!=NULL ) {
        deleted = (virDomainDestroy(domain)+1) ? true : false;
        if (!deleted) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.name = name;
    result.result = deleted;
    result.msg.append(QString("'%1' Domain %2 Destroyed.").arg(name).arg((deleted)?"":"don't"));
    return result;
}
Result DomControlThread::resetDomain()
{
    Result result;
    QString name = args.first();
    bool invoked = false;
    // extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;

    virDomainPtr domain = virDomainLookupByName(currWorkConnect, name.toUtf8().data());

    if ( domain!=NULL ) {
        invoked = (virDomainReset(domain, flags)+1) ? true : false;
        if (!invoked) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.name = name;
    result.result = invoked;
    result.msg.append(QString("'%1' Domain %2 reset.").arg(name).arg((invoked)?"":"don't"));
    return result;
}
Result DomControlThread::rebootDomain()
{
    Result result;
    QString name = args.first();
    bool invoked = false;
    unsigned int flags =
            VIR_DOMAIN_REBOOT_DEFAULT |
            VIR_DOMAIN_REBOOT_ACPI_POWER_BTN |
            VIR_DOMAIN_REBOOT_GUEST_AGENT |
            VIR_DOMAIN_REBOOT_INITCTL |
            VIR_DOMAIN_REBOOT_SIGNAL;

    virDomainPtr domain = virDomainLookupByName(currWorkConnect, name.toUtf8().data());

    if ( domain!=NULL ) {
        invoked = (virDomainReboot(domain, flags)+1) ? true : false;
        if (!invoked) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.name = name;
    result.result = invoked;
    result.msg.append(QString("'%1' Domain %2 reboot.").arg(name).arg((invoked)?"":"don't"));
    return result;
}
Result DomControlThread::shutdownDomain()
{
    Result result;
    QString name = args.first();
    bool invoked = false;
    unsigned int flags =
            VIR_DOMAIN_SHUTDOWN_DEFAULT |
            VIR_DOMAIN_SHUTDOWN_ACPI_POWER_BTN |
            VIR_DOMAIN_SHUTDOWN_GUEST_AGENT |
            VIR_DOMAIN_SHUTDOWN_INITCTL |
            VIR_DOMAIN_SHUTDOWN_SIGNAL;

    virDomainPtr domain = virDomainLookupByName(currWorkConnect, name.toUtf8().data());

    if ( domain!=NULL ) {
        invoked = (virDomainShutdownFlags(domain, flags)+1) ? true : false;
        if (!invoked) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.name = name;
    result.result = invoked;
    result.msg.append(QString("'%1' Domain %2 shutdown.").arg(name).arg((invoked)?"":"don't"));
    return result;
}
Result DomControlThread::saveDomain()
{
    Result result;
    QString name = args.first();
    args.removeFirst();
    const char *to = args.first().toUtf8().data();
    const char *dxml = NULL;
    QString state = args.last();
    bool invoked = false;
    unsigned int flags = VIR_DOMAIN_SAVE_BYPASS_CACHE;
    if ( state=="RUNNING" ) {
        flags = flags | VIR_DOMAIN_SAVE_RUNNING;
    } else if ( state=="PAUSED" ) {
        flags = flags | VIR_DOMAIN_SAVE_PAUSED;
    };

    virDomainPtr domain = virDomainLookupByName(currWorkConnect, name.toUtf8().data());

    if ( domain!=NULL ) {
        invoked = (virDomainSaveFlags(domain, to, dxml, flags)+1) ? true : false;
        if (!invoked) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.name = name;
    result.result = invoked;
    result.msg.append(QString("'%1' Domain %2 saved.").arg(name).arg((invoked)?"":"don't"));
    return result;
}
Result DomControlThread::restoreDomain()
{
    Result result;
    QString name = args.first();
    args.removeFirst();
    const char *from = args.first().toUtf8().data();
    const char *dxml = NULL;
    QString to_state = args.last();
    bool invoked = false;
    unsigned int flags = VIR_DOMAIN_SAVE_BYPASS_CACHE;
    if ( to_state=="RUNNING" ) {
        flags = flags | VIR_DOMAIN_SAVE_RUNNING;
    } else if ( to_state=="PAUSED" ) {
        flags = flags | VIR_DOMAIN_SAVE_PAUSED;
    };

    if ( currWorkConnect!=NULL ) {
        invoked = (virDomainRestoreFlags(currWorkConnect, from, dxml, flags)+1) ? true : false;
        if (!invoked) sendConnErrors();
    };
    result.name = name;
    result.result = invoked;
    result.msg.append(QString("'%1' Domain %2 restored.").arg(name).arg((invoked)?"":"don't"));
    return result;
}
Result DomControlThread::undefineDomain()
{
    Result result;
    QString name = args.first();

    bool deleted = false;
    virDomainPtr domain = virDomainLookupByName(currWorkConnect, name.toUtf8().data());
    if ( domain!=NULL ) {
        deleted = (virDomainUndefine(domain)+1) ? true : false;
        if (!deleted) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.name = name;
    result.result = deleted;
    result.msg.append(QString("'%1' Domain %2 Undefined.").arg(name).arg((deleted)?"":"don't"));
    return result;
}
Result DomControlThread::changeAutoStartDomain()
{
    Result result;
    QString name = args.first();
    result.name = name;
    int autostart;
    if ( args.count()<2 || args.at(1).isEmpty() ) {
        result.msg.append("Incorrect parameters.");
        return result;
    } else {
        bool converted;
        int res = args.at(1).toInt(&converted);
        if (converted) autostart = (res) ? 1 : 0;
        else {
            result.msg.append("Incorrect parameters.");
            return result;
        };
    };

    bool set = false;
    virDomainPtr domain = virDomainLookupByName(currWorkConnect, name.toUtf8().data());
    if ( domain!=NULL ) {
        set = (virDomainSetAutostart(domain, autostart)+1) ? true : false;
        if (!set) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.name = name;
    result.result = set;
    result.msg.append(QString("'%1' Domain autostart %2 Set.").arg(name).arg((set)?"":"don't"));
    return result;
}
Result DomControlThread::getDomainXMLDesc()
{
    Result result;
    QString name = args.first();

    bool read = false;
    char *Returns = NULL;
    virDomainPtr domain = virDomainLookupByName(currWorkConnect, name.toUtf8().data());
    if ( domain!=NULL ) {
        Returns = (virDomainGetXMLDesc(domain, VIR_DOMAIN_XML_INACTIVE));
        if ( Returns==NULL ) sendConnErrors();
        else read = true;
        virDomainFree(domain);
    } else sendConnErrors();
    QTemporaryFile f;
    f.setAutoRemove(false);
    f.setFileTemplate(QString("%1%2XML_Desc-XXXXXX.xml").arg(QDir::tempPath()).arg(QDir::separator()));
    read = f.open();
    if (read) f.write(Returns);
    result.msg.append(f.fileName());
    f.close();
    free(Returns);
    result.name = name;
    result.result = read;
    result.msg.append(QString("'%1' Domain %2 XML'ed").arg(name).arg((read)?"":"don't"));
    return result;
}

void DomControlThread::sendConnErrors()
{
    virtErrors = virConnGetLastError(currWorkConnect);
    if ( virtErrors!=NULL ) {
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                       .arg(QString().fromUtf8(virtErrors->message)) );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void DomControlThread::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL )
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code)
                       .arg(QString().fromUtf8(virtErrors->message)) );
    virResetLastError();
}
