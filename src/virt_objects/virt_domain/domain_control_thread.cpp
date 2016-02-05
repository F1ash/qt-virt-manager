#include "domain_control_thread.h"

DomControlThread::DomControlThread(QObject *parent) :
    ControlThread(parent)
{
}

/* public slots */
void DomControlThread::execAction(uint _num, TASK _task)
{
    number = _num;
    task = _task;
    keep_alive = false;
    if ( NULL!=task.srcConnPtr ) {
        // for new virConnect usage create the new virConnectRef[erence]
        int ret = virConnectRef(*task.srcConnPtr);
        if ( ret<0 ) {
            task.srcConnPtr = NULL;
            sendConnErrors();
        } else
            keep_alive = true;
    };
    if ( keep_alive && !isRunning() ) {
        ptr_ConnPtr = task.srcConnPtr;
        start();
    } else {
        Result result;
        result.type   = "domain";
        result.number = number;
        result.action = _EMPTY_ACTION;
        emit resultData(result);
    };
}

/* private slots */
void DomControlThread::run()
{
    Result result;
    switch (task.action) {
    case GET_ALL_ENTITY_STATE :
        result = getAllDomainList();
        break;
    case CREATE_ENTITY :
        result = createDomain();
        break;
    case DEFINE_ENTITY :
        result = defineDomain();
        break;
    case EDIT_ENTITY :
        result = getDomainXMLDesc();
        break;
    case START_ENTITY :
        result = startDomain();
        break;
    case PAUSE_ENTITY :
        result = pauseDomain();
        break;
    case DESTROY_ENTITY :
        result = destroyDomain();
        break;
    case RESET_ENTITY :
        result = resetDomain();
        break;
    case REBOOT_ENTITY :
        result = rebootDomain();
        break;
    case SHUTDOWN_ENTITY :
        result = shutdownDomain();
        break;
    case SAVE_ENTITY :
        result = saveDomain();
        break;
    case RESTORE_ENTITY :
        result = restoreDomain();
        break;
    case UNDEFINE_ENTITY :
        result = undefineDomain();
        break;
    case CHANGE_ENTITY_AUTOSTART :
        result = changeAutoStartDomain();
        break;
    case GET_XML_DESCRIPTION :
        result = getDomainXMLDesc();
        break;
    case MIGRATE_ENTITY :
        result = migrateDomain();
        break;
    case CREATE_DOMAIN_SNAPSHOT :
        result = createSnapshoteDomain();
        break;
    case REVERT_TO_DOMAIN_SNAPSHOT :
        result = revertSnapshoteDomain();
        break;
    case DELETE_DOMAIN_SNAPSHOT :
        result = deleteSnapshoteDomain();
        break;
    default:
        break;
    };
    // task.srcConnPtr reference will closed in destructor as ptr_ConnPtr
    //virConnectClose(*task.srcConnPtr);
    result.type   = "domain";
    result.number = number;
    result.action = task.action;
    emit resultData(result);
}
Result DomControlThread::getAllDomainList()
{
    Result result;
    QStringList domainList;
    if ( task.srcConnPtr!=NULL && keep_alive ) {
        virDomainPtr *domains = NULL;
        unsigned int flags = VIR_CONNECT_LIST_DOMAINS_ACTIVE |
                             VIR_CONNECT_LIST_DOMAINS_INACTIVE;
        // the number of domains found or -1 and sets domains to NULL in case of error.
        int ret = virConnectListAllDomains(
                    *task.srcConnPtr, &domains, flags);
        if ( ret<0 ) {
            result.err = sendConnErrors();
            result.result = false;
            result.msg = domainList;
            return result;
        };

        // therefore correctly to use for() command, because domains[0] can not exist.
        for (int i = 0; i < ret; i++) {
            QStringList currentAttr;
            QString autostartStr;
            int is_autostart = 0;
            if (virDomainGetAutostart(domains[i], &is_autostart) < 0) {
                autostartStr.append("no autostart");
            } else autostartStr.append( is_autostart ? "yes" : "no" );
            int state;
            int reason;
            // flags : extra flags; not used yet, so callers should always pass 0
            flags = 0;
            QString domainState;
            if ( virDomainGetState(domains[i], &state, &reason, flags)+1 ) {
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
            currentAttr<< QString::fromUtf8( virDomainGetName(domains[i]) )
                       << QString("%1:%2")
                          .arg( virDomainIsActive(domains[i]) ? "active" : "inactive" )
                          .arg(domainState)
                       << autostartStr
                       << QString( virDomainIsPersistent(domains[i]) ? "yes" : "no" );
            domainList.append(currentAttr.join(DFR));
            //qDebug()<<currentAttr;
            /*
            virDomainInfo info;
            if ( virDomainGetInfo(domains[i], &info)+1 ) {
                qDebug()<<info.state
                        <<info.maxMem
                        <<info.memory
                        <<info.nrVirtCpu
                        <<info.cpuTime;
            };
            */
            virDomainFree(domains[i]);
        };
        if (domains) free(domains);
    };
    result.result = true;
    result.msg = domainList;
    return result;
}
Result DomControlThread::createDomain()
{
    Result result;
    QString path = task.args.path;
    QByteArray xmlData;
    QFile f;
    f.setFileName(path);
    if ( !f.open(QIODevice::ReadOnly) ) {
        QString msg = QString("File \"%1\"\nnot opened.").arg(path);
        emit errorMsg( msg, number );
        result.err = msg;
        return result;
    };
    xmlData = f.readAll();
    f.close();
    virDomainPtr domain = virDomainCreateXML(
                *task.srcConnPtr, xmlData.data(), VIR_DOMAIN_START_AUTODESTROY);
    if ( domain==NULL ) {
        result.err = sendConnErrors();
        return result;
    };
    result.name = QString().fromUtf8( virDomainGetName(domain) );
    result.result = true;
    result.msg.append(
                QString("'<b>%1</b>' Domain from\n\"%2\"\nis created.")
                .arg(result.name).arg(path));
    virDomainFree(domain);
    return result;
}
Result DomControlThread::defineDomain()
{
    Result result;
    QString path = task.args.path;
    QByteArray xmlData;
    QFile f;
    f.setFileName(path);
    if ( !f.open(QIODevice::ReadOnly) ) {
        QString msg = QString("File \"%1\"\nnot opened.").arg(path);
        emit errorMsg( msg, number );
        result.err = msg;
        return result;
    };
    xmlData = f.readAll();
    f.close();
    virDomainPtr domain = virDomainDefineXML(
                *task.srcConnPtr, xmlData.data());
    if ( domain==NULL ) {
        result.err = sendConnErrors();
        return result;
    };
    result.name = QString().fromUtf8( virDomainGetName(domain) );
    result.result = true;
    result.msg.append(
                QString("'<b>%1</b>' Domain from\n\"%2\"\nis defined.")
                .arg(result.name).arg(path));
    virDomainFree(domain);
    return result;
}
Result DomControlThread::startDomain()
{
    Result result;
    QString name = task.object;
    bool started = false;
    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( domain!=NULL ) {
        started = (virDomainCreate(domain)+1) ? true : false;
        if (!started)
            result.err = sendConnErrors();
        virDomainFree(domain);
    } else
        result.err = sendConnErrors();
    result.name = name;
    result.result = started;
    result.msg.append(
                QString("'<b>%1</b>' Domain %2 Started.")
                .arg(name).arg((started)?"":"don't"));
    return result;
}
Result DomControlThread::pauseDomain()
{
    Result result;
    QString name = task.object;
    QString state = task.args.state;
    bool invoked = false;

    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());

    if ( domain!=NULL ) {
        if ( state=="RUNNING" ) {
            invoked = (virDomainSuspend(domain)+1) ? true : false;
            if (!invoked)
                result.err = sendConnErrors();
        } else if ( state=="PAUSED" ) {
            invoked = (virDomainResume(domain)+1) ? true : false;
            if (!invoked)
                result.err = sendConnErrors();
        } else 0;
        virDomainFree(domain);
    } else
        result.err = sendConnErrors();
    result.name = name;
    result.result = invoked;
    result.msg.append(
                QString("'<b>%1</b>' Domain state %2 changed.")
                .arg(name).arg((invoked)?"":"don't"));
    return result;
}
Result DomControlThread::destroyDomain()
{
    Result result;
    QString name = task.object;
    bool deleted = false;
    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( domain!=NULL ) {
        deleted = (virDomainDestroy(domain)+1) ? true : false;
        if (!deleted)
            result.err = sendConnErrors();
        virDomainFree(domain);
    } else
        result.err = sendConnErrors();
    result.name = name;
    result.result = deleted;
    result.msg.append(
                QString("'<b>%1</b>' Domain %2 Destroyed.")
                .arg(name).arg((deleted)?"":"don't"));
    return result;
}
Result DomControlThread::resetDomain()
{
    Result result;
    QString name = task.object;
    bool invoked = false;
    // extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;

    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());

    if ( domain!=NULL ) {
        invoked = (virDomainReset(domain, flags)+1) ? true : false;
        if (!invoked)
            result.err = sendConnErrors();
        virDomainFree(domain);
    } else
        result.err = sendConnErrors();
    result.name = name;
    result.result = invoked;
    result.msg.append(
                QString("'<b>%1</b>' Domain %2 reset.")
                .arg(name).arg((invoked)?"":"don't"));
    return result;
}
Result DomControlThread::rebootDomain()
{
    Result result;
    QString name = task.object;
    bool invoked = false;
    unsigned int flags =
            VIR_DOMAIN_REBOOT_DEFAULT |
            VIR_DOMAIN_REBOOT_ACPI_POWER_BTN |
            VIR_DOMAIN_REBOOT_GUEST_AGENT |
            VIR_DOMAIN_REBOOT_INITCTL |
            VIR_DOMAIN_REBOOT_SIGNAL;

    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());

    if ( domain!=NULL ) {
        invoked = (virDomainReboot(domain, flags)+1) ? true : false;
        if (!invoked)
            result.err = sendConnErrors();
        virDomainFree(domain);
    } else
        result.err = sendConnErrors();
    result.name = name;
    result.result = invoked;
    result.msg.append(
                QString("'<b>%1</b>' Domain %2 reboot.")
                .arg(name).arg((invoked)?"":"don't"));
    return result;
}
Result DomControlThread::shutdownDomain()
{
    Result result;
    QString name = task.object;
    bool invoked = false;
    unsigned int flags =
            VIR_DOMAIN_SHUTDOWN_DEFAULT |
            VIR_DOMAIN_SHUTDOWN_ACPI_POWER_BTN |
            VIR_DOMAIN_SHUTDOWN_GUEST_AGENT |
            VIR_DOMAIN_SHUTDOWN_INITCTL |
            VIR_DOMAIN_SHUTDOWN_SIGNAL;

    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());

    if ( domain!=NULL ) {
        invoked = (virDomainShutdownFlags(
                       domain, flags)+1) ? true : false;
        if (!invoked)
            result.err = sendConnErrors();
        virDomainFree(domain);
    } else
        result.err = sendConnErrors();
    result.name = name;
    result.result = invoked;
    result.msg.append(
                QString("'<b>%1</b>' Domain %2 shutdown.")
                .arg(name).arg((invoked)?"":"don't"));
    return result;
}
Result DomControlThread::saveDomain()
{
    Result result;
    QString name = task.object;
    const char *to = task.args.path.toUtf8().data();
    const char *dxml = NULL;
    QString state = task.args.state;
    bool invoked = false;
    unsigned int flags = VIR_DOMAIN_SAVE_BYPASS_CACHE;
    if ( state=="RUNNING" ) {
        flags = flags | VIR_DOMAIN_SAVE_RUNNING;
    } else if ( state=="PAUSED" ) {
        flags = flags | VIR_DOMAIN_SAVE_PAUSED;
    };

    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());

    if ( domain!=NULL ) {
        invoked = (virDomainSaveFlags(
                       domain, to, dxml, flags)+1)
                ? true : false;
        if (!invoked)
            result.err = sendConnErrors();
        virDomainFree(domain);
    } else
        result.err = sendConnErrors();
    result.name = name;
    result.result = invoked;
    result.msg.append(
                QString("'<b>%1</b>' Domain %2 saved.")
                .arg(name).arg((invoked)?"":"don't"));
    return result;
}
Result DomControlThread::restoreDomain()
{
    Result result;
    QString name = task.object;
    const char *from = task.args.path.toUtf8().data();
    const char *dxml = NULL;
    QString to_state = task.args.state;
    bool invoked = false;
    unsigned int flags = VIR_DOMAIN_SAVE_BYPASS_CACHE;
    if ( to_state=="RUNNING" ) {
        flags = flags | VIR_DOMAIN_SAVE_RUNNING;
    } else if ( to_state=="PAUSED" ) {
        flags = flags | VIR_DOMAIN_SAVE_PAUSED;
    };

    if ( task.srcConnPtr!=NULL ) {
        invoked = (virDomainRestoreFlags(
                       *task.srcConnPtr, from, dxml, flags)+1)
                ? true : false;
        if (!invoked)
            result.err = sendConnErrors();
    };
    result.name = name;
    result.result = invoked;
    result.msg.append(
                QString("'<b>%1</b>' Domain %2 restored.")
                .arg(name).arg((invoked)?"":"don't"));
    return result;
}
Result DomControlThread::undefineDomain()
{
    Result result;
    QString name = task.object;
    bool deleted = false;
    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( domain!=NULL ) {
        deleted = (virDomainUndefine(domain)+1) ? true : false;
        if (!deleted)
            result.err = sendConnErrors();
        virDomainFree(domain);
    } else
        result.err = sendConnErrors();
    result.name = name;
    result.result = deleted;
    result.msg.append(
                QString("'<b>%1</b>' Domain %2 Undefined.")
                .arg(name).arg((deleted)?"":"don't"));
    return result;
}
Result DomControlThread::changeAutoStartDomain()
{
    Result result;
    QString name = task.object;
    result.name = name;
    int autostart = task.args.sign;

    bool set = false;
    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( domain!=NULL ) {
        set = (virDomainSetAutostart(
                   domain, autostart)+1) ? true : false;
        if (!set)
            result.err = sendConnErrors();
        virDomainFree(domain);
    } else
        result.err = sendConnErrors();
    result.name = name;
    result.result = set;
    result.msg.append(
                QString("'<b>%1</b>' Domain autostart %2 Set.")
                .arg(name).arg((set)?"":"don't"));
    return result;
}
Result DomControlThread::getDomainXMLDesc()
{
    Result result;
    QString name = task.object;
    unsigned int flags = task.args.sign;

    bool read = false;
    char *Returns = NULL;
    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( domain!=NULL ) {
        Returns = (virDomainGetXMLDesc(domain, flags));
        if ( Returns==NULL )
            result.err = sendConnErrors();
        else read = true;
        virDomainFree(domain);
    } else
        result.err = sendConnErrors();
    QTemporaryFile f;
    f.setAutoRemove(false);
    f.setFileTemplate(
                QString("%1%2XML_Desc-XXXXXX.xml")
                .arg(QDir::tempPath())
                .arg(QDir::separator()));
    read = f.open();
    if (read) f.write(Returns);
    result.fileName.append(f.fileName());
    f.close();
    if ( Returns!=NULL ) free(Returns);
    result.name = name;
    result.result = read;
    result.msg.append(
                QString("'<b>%1</b>' Domain %2 XML'ed")
                .arg(name).arg((read)?"":"don't"));
    return result;
}
Result DomControlThread::migrateDomain()
{
    Result result;
    bool migrated = false;
    result.name = task.object;
    unsigned int flags = task.args.sign;
    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, result.name.toUtf8().constData());
    //const char *uri = ( args[2].isEmpty() ) ? NULL : args[2].toUtf8().constData();
    int maxDownTime = task.args.offset;
    int bdw = task.args.size;
    if ( domain!=NULL ) {
        // virDomainMigrateSetMaxDowntime
        // flags: extra flags; not used yet, so callers should always pass 0
        virDomainMigrateSetMaxDowntime(domain, maxDownTime, 0);
        if ( NULL!=task.args.destConnPtr ) {
            qDebug()<<"migrate to exist connect";
            virDomainPtr newDomain =
            virDomainMigrate(domain,
                             *task.args.destConnPtr,
                             flags,
                             task.args.object.toUtf8().data(),
                             task.args.path.toUtf8().data(),
                             bdw);
            migrated = NULL!=newDomain;
            if (migrated) virDomainFree( newDomain );
        } else {
            qDebug()<< "migrate to URI";
            migrated = (virDomainMigrateToURI(
                            domain,
                            task.args.path.toUtf8().data(),
                            flags,
                            task.args.object.toUtf8().data(),
                            bdw)
                        +1)?true:false;
        };
        virDomainFree(domain);
        if ( !migrated )
            result.err = sendConnErrors();
    } else
        result.err = sendConnErrors();
    result.result = migrated;
    result.msg.append(
                QString("'<b>%1</b>' Domain %2 Migrated.")
                .arg(result.name).arg((migrated)?"":"don't"));
    //if ( task.args.destConnPtr ) task.args.destConnPtr = NULL;
    return result;
}
Result DomControlThread::createSnapshoteDomain()
{
    Result result;
    bool snapped = false;
    QString domName = task.object;
    result.name = domName;
    unsigned int flags = task.args.sign;
    QByteArray _xmlDesc;
    _xmlDesc.append(task.args.object);
    const char *xmlDesc = _xmlDesc.data();
    //qDebug()<<xmlDesc<<flags;
    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, domName.toUtf8().data());
    if ( NULL==domain ) {
        result.err = sendConnErrors();
    } else {
        virDomainSnapshotPtr snapshot =
                virDomainSnapshotCreateXML(domain, xmlDesc, flags);
        if ( NULL==snapshot ) {
            result.err = sendConnErrors();
        } else {
            snapped = true;
            virDomainSnapshotFree(snapshot);
        };
        virDomainFree(domain);
    };
    result.result = snapped;
    result.msg.append(
                QString("'<b>%1</b>' Domain %2 snapped.")
                .arg(result.name).arg((snapped)?"":"don't"));
    return result;
}
Result DomControlThread::revertSnapshoteDomain()
{
    Result result;
    bool reverted = false;
    QString domName = task.object;
    result.name = task.args.object;
    QString snapshotName(task.args.object);
    unsigned int flags = task.args.sign;
    //qDebug()<<snapshotName<<flags;
    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, domName.toUtf8().data());
    if ( NULL==domain ) {
        result.err = sendConnErrors();
    } else {
        // flags: extra flags; not used yet, so callers should always pass 0
        virDomainSnapshotPtr snapshot = virDomainSnapshotLookupByName(
                    domain, snapshotName.toUtf8().data(), 0);
        if ( NULL==snapshot ) {
            result.err = sendConnErrors();
       } else {
           int ret = virDomainRevertToSnapshot(snapshot, flags);
           if ( ret<0 ) {
               result.err = sendConnErrors();
            } else {
                reverted = true;
            };
            virDomainSnapshotFree(snapshot);
        };
        virDomainFree(domain);
    };
    result.msg.append(
                QString("'<b>%1</b>' snapshot in <b>%2</b> Domain %3 reverted.")
                .arg(result.name).arg(domName).arg((reverted)?"":"don't"));
    result.result = reverted;
    return result;
}
Result DomControlThread::deleteSnapshoteDomain()
{
    Result result;
    bool deleted = false;
    QString domName = task.object;
    result.name = task.args.object;
    QString snapshotName(task.args.object);
    unsigned int flags = task.args.sign;
    //qDebug()<<snapshotName<<flags;
    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, domName.toUtf8().data());
    if ( NULL==domain ) {
        result.err = sendConnErrors();
    } else {
        // flags: extra flags; not used yet, so callers should always pass 0
        virDomainSnapshotPtr snapshot =
                virDomainSnapshotLookupByName(
                    domain, snapshotName.toUtf8().data(), 0);
        if ( NULL==snapshot ) {
            result.err = sendConnErrors();
        } else {
            int ret = virDomainSnapshotDelete(snapshot, flags);
            if ( ret<0 ) {
                result.err = sendConnErrors();
            } else {
                deleted = true;
            };
            virDomainSnapshotFree(snapshot);
        };
        virDomainFree(domain);
    };
    result.msg.append(
                QString("'<b>%1</b>' snapshot in <b>%2</b> Domain %3 deleted.")
                .arg(result.name).arg(domName).arg((deleted)?"":"don't"));
    result.result = deleted;
    return result;
}
