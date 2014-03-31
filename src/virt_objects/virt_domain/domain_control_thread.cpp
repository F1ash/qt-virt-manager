#include "domain_control_thread.h"

DomControlThread::DomControlThread(QObject *parent) :
    QThread(parent)
{
    qRegisterMetaType<DomActions>("DomActions");
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
    action = DOM_EMPTY_ACTION;
    args.clear();
    if ( keep_alive && !isRunning() ) {
        action = act;
        args = _args;
        start();
    };
}

/* private slots */
void DomControlThread::run()
{
    QStringList result;
    switch (action) {
    case GET_ALL_DOMAIN :
        result.append(getAllDomainList());
        break;
    case CREATE_DOMAIN :
        result.append(createDomain());
        break;
    case DEFINE_DOMAIN :
        result.append(defineDomain());
        break;
    case START_DOMAIN :
        result.append(startDomain());
        break;
    case PAUSE_DOMAIN :
        result.append(pauseDomain());
        break;
    case DESTROY_DOMAIN :
        result.append(destroyDomain());
        break;
    case RESET_DOMAIN :
        result.append(resetDomain());
        break;
    case REBOOT_DOMAIN :
        result.append(rebootDomain());
        break;
    case SHUTDOWN_DOMAIN :
        result.append(shutdownDomain());
        break;
    case SAVE_DOMAIN :
        result.append(saveDomain());
        break;
    case UNDEFINE_DOMAIN :
        result.append(undefineDomain());
        break;
    case CHANGE_DOM_AUTOSTART :
        result.append(changeAutoStartDomain());
        break;
    case GET_DOM_XML_DESC :
        result.append(getDomainXMLDesc());
        break;
    default:
        break;
    };
    emit resultData(action, result);
}
QStringList DomControlThread::getAllDomainList()
{
    QStringList domainList;
    if ( currWorkConnect!=NULL && keep_alive ) {
        virDomainPtr *domain;
        unsigned int flags = VIR_CONNECT_LIST_DOMAINS_ACTIVE |
                             VIR_CONNECT_LIST_DOMAINS_INACTIVE;
        int ret = virConnectListAllDomains( currWorkConnect, &domain, flags);
        if ( ret<0 ) {
            sendConnErrors();
            free(domain);
            return domainList;
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
            currentAttr<< QString( virDomainGetName(domain[i]) )
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
    return domainList;
}
QStringList DomControlThread::createDomain()
{
    QStringList result;
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
    result.append(QString("'%1' Domain from\n\"%2\"\nis created.").arg(virDomainGetName(domain)).arg(path));
    virDomainFree(domain);
    return result;
}
QStringList DomControlThread::defineDomain()
{
    QStringList result;
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
    result.append(QString("'%1' Domain from\n\"%2\"\nis defined.").arg(virDomainGetName(domain)).arg(path));
    virDomainFree(domain);
    return result;
}
QStringList DomControlThread::startDomain()
{
    QStringList result;
    QString name = args.first();
    /*
    virDomainPtr *domain;
    unsigned int flags = VIR_CONNECT_LIST_DOMAINS_ACTIVE |
                         VIR_CONNECT_LIST_DOMAINS_INACTIVE;
    int ret = virConnectListAllDomains( currWorkConnect, &domain, flags);
    if ( ret<0 ) {
        sendConnErrors();
        free(domain);
        return result;
    };
    //qDebug()<<QString(virConnectGetURI(currWorkConnect));
    */

    bool started = false;
    /*
    int i = 0;
    while ( domain[i] != NULL ) {
        QString currNetName = QString( virDomainGetName(domain[i]) );
        if ( !started && currNetName==name ) {
            started = (virDomainCreate(domain[i])+1) ? true : false;
            if (!started) sendGlobalErrors();
        };
        virDomainFree(domain[i]);
        i++;
    };
    free(domain);
    */

    virDomainPtr domain = virDomainLookupByName(currWorkConnect, name.toUtf8().data());

    if ( domain!=NULL ) {
        started = (virDomainCreate(domain)+1) ? true : false;
        if (!started) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.append(QString("'%1' Domain %2 Started.").arg(name).arg((started)?"":"don't"));
    return result;
}
QStringList DomControlThread::pauseDomain()
{
    QStringList result;
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
    result.append(QString("'%1' Domain state %2 changed.").arg(name).arg((invoked)?"":"don't"));
    return result;
}
QStringList DomControlThread::destroyDomain()
{
    QStringList result;
    QString name = args.first();
    /*
    virDomainPtr *domain;
    unsigned int flags = VIR_CONNECT_LIST_DOMAINS_ACTIVE |
                         VIR_CONNECT_LIST_DOMAINS_INACTIVE;
    int ret = virConnectListAllDomains( currWorkConnect, &domain, flags);
    if ( ret<0 ) {
        sendConnErrors();
        free(domain);
        return result;
    };
    //qDebug()<<QString(virConnectGetURI(currWorkConnect));
    */

    bool deleted = false;
    /*
    int i = 0;
    while ( domain[i] != NULL ) {
        QString currDomName = QString( virDomainGetName(domain[i]) );
        if ( !deleted && currDomName==name ) {
            deleted = (virDomainDestroy(domain[i])+1) ? true : false;
            if (!deleted) sendGlobalErrors();
        };
        qDebug()<<QVariant(deleted).toString()<<currDomName<<name;
        virDomainFree(domain[i]);
        i++;
    };
    free(domain);
    */
    virDomainPtr domain = virDomainLookupByName(currWorkConnect, name.toUtf8().data());
    if ( domain!=NULL ) {
        deleted = (virDomainDestroy(domain)+1) ? true : false;
        if (!deleted) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.append(QString("'%1' Domain %2 Destroyed.").arg(name).arg((deleted)?"":"don't"));
    return result;
}
QStringList DomControlThread::resetDomain()
{
    QStringList result;
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
    result.append(QString("'%1' Domain %2 reset.").arg(name).arg((invoked)?"":"don't"));
    return result;
}
QStringList DomControlThread::rebootDomain()
{
    QStringList result;
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
    result.append(QString("'%1' Domain %2 reboot.").arg(name).arg((invoked)?"":"don't"));
    return result;
}
QStringList DomControlThread::shutdownDomain()
{
    QStringList result;
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
    result.append(QString("'%1' Domain %2 shutdown.").arg(name).arg((invoked)?"":"don't"));
    return result;
}
QStringList DomControlThread::saveDomain()
{
    QStringList result;
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
    result.append(QString("'%1' Domain %2 saved.").arg(name).arg((invoked)?"":"don't"));
    return result;
}
QStringList DomControlThread::undefineDomain()
{
    QStringList result;
    QString name = args.first();
    /*
    virDomainPtr *domain;
    unsigned int flags = VIR_CONNECT_LIST_DOMAINS_ACTIVE |
                         VIR_CONNECT_LIST_DOMAINS_INACTIVE;
    int ret = virConnectListAllDomains( currWorkConnect, &domain, flags);
    if ( ret<0 ) {
        sendConnErrors();
        free(domain);
        return result;
    };
    //qDebug()<<QString(virConnectGetURI(currWorkConnect));
    */

    bool deleted = false;
    /*
    int i = 0;
    while ( domain[i] != NULL ) {
        QString currDomName = QString( virDomainGetName(domain[i]) );
        if ( !deleted && currDomName==name ) {
            deleted = (virDomainUndefine(domain[i])+1) ? true : false;
            if (!deleted) sendGlobalErrors();
        };
        qDebug()<<QVariant(deleted).toString()<<currDomName<<name;
        virDomainFree(domain[i]);
        i++;
    };
    free(domain);
    */
    virDomainPtr domain = virDomainLookupByName(currWorkConnect, name.toUtf8().data());
    if ( domain!=NULL ) {
        deleted = (virDomainUndefine(domain)+1) ? true : false;
        if (!deleted) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.append(QString("'%1' Domain %2 Undefined.").arg(name).arg((deleted)?"":"don't"));
    return result;
}
QStringList DomControlThread::changeAutoStartDomain()
{
    QStringList result;
    QString name = args.first();
    int autostart;
    if ( args.count()<2 || args.at(1).isEmpty() ) {
        result.append("Incorrect parameters.");
        return result;
    } else {
        bool converted;
        int res = args.at(1).toInt(&converted);
        if (converted) autostart = (res) ? 1 : 0;
        else {
            result.append("Incorrect parameters.");
            return result;
        };
    };
    /*
    virDomainPtr *domain;
    unsigned int flags = VIR_CONNECT_LIST_DOMAINS_ACTIVE |
                         VIR_CONNECT_LIST_DOMAINS_INACTIVE;
    int ret = virConnectListAllDomains( currWorkConnect, &domain, flags);
    if ( ret<0 ) {
        sendConnErrors();
        free(domain);
        return result;
    };
    //qDebug()<<QString(virConnectGetURI(currWorkConnect));
    */

    bool set = false;
    /*
    int i = 0;
    while ( domain[i] != NULL ) {
        QString currNetName = QString( virDomainGetName(domain[i]) );
        if ( !set && currNetName==name ) {
            set = (virDomainSetAutostart(domain[i], autostart)+1) ? true : false;
            if (!set) sendGlobalErrors();
        };
        virDomainFree(domain[i]);
        i++;
    };
    free(domain);
    */
    virDomainPtr domain = virDomainLookupByName(currWorkConnect, name.toUtf8().data());
    if ( domain!=NULL ) {
        set = (virDomainSetAutostart(domain, autostart)+1) ? true : false;
        if (!set) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.append(QString("'%1' Domain autostart %2 Set.").arg(name).arg((set)?"":"don't"));
    return result;
}
QStringList DomControlThread::getDomainXMLDesc()
{
    QStringList result;
    QString name = args.first();
    /*
    virDomainPtr *domain;
    unsigned int flags = VIR_CONNECT_LIST_DOMAINS_ACTIVE |
                         VIR_CONNECT_LIST_DOMAINS_INACTIVE;
    int ret = virConnectListAllDomains( currWorkConnect, &domain, flags);
    if ( ret<0 ) {
        sendConnErrors();
        free(domain);
        return result;
    };
    //qDebug()<<QString(virConnectGetURI(currWorkConnect));

    int i = 0;
    */
    bool read = false;
    char *Returns = NULL;
    /*
    while ( domain[i] != NULL ) {
        QString currNetName = QString( virDomainGetName(domain[i]) );
        if ( !read && currNetName==name ) {
            Returns = (virDomainGetXMLDesc(domain[i], VIR_DOMAIN_XML_INACTIVE));
            if ( Returns==NULL ) sendGlobalErrors();
            else read = true;
        };
        virDomainFree(domain[i]);
        i++;
    };
    free(domain);
    */
    virDomainPtr domain = virDomainLookupByName(currWorkConnect, name.toUtf8().data());
    if ( domain!=NULL ) {
        Returns = (virDomainGetXMLDesc(domain, VIR_DOMAIN_XML_INACTIVE));
        if ( Returns==NULL ) sendConnErrors();
        else read = true;
        virDomainFree(domain);
    } else sendConnErrors();
    QTemporaryFile f;
    f.setAutoRemove(false);
    read = f.open();
    if (read) f.write(Returns);
    result.append(f.fileName());
    f.close();
    free(Returns);
    result.append(QString("'%1' Domain %2 XML'ed").arg(name).arg((read)?"":"don't"));
    return result;
}

void DomControlThread::sendConnErrors()
{
    virtErrors = virConnGetLastError(currWorkConnect);
    if ( virtErrors!=NULL ) {
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code).arg(virtErrors->message) );
        virResetError(virtErrors);
    } else sendGlobalErrors();
}
void DomControlThread::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL )
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code).arg(virtErrors->message) );
    virResetLastError();
}
