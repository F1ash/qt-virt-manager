#include "domain_control_thread.h"

DomControlThread::DomControlThread(QObject *parent) :
    ControlThread(parent)
{
}

/* public slots */
void DomControlThread::execAction(Actions act, QStringList _args)
{
    if ( keep_alive && !isRunning() ) {
        action = act;
        args = _args;
        if ( NULL!=currWorkConnection ) start();
        else {
            Result result;
            result.type   = "domain";
            result.number = number;
            result.action = _EMPTY_ACTION;
            emit resultData(result);
        };
    };
}
void DomControlThread::setMigrateConnect(virConnectPtr conn)
{
    destConnect = conn;
}

/* private slots */
void DomControlThread::run()
{
    Result result;
    switch (action) {
    case GET_ALL_ENTITY :
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
    result.type   = "domain";
    result.number = number;
    result.action = action;
    emit resultData(result);
}
Result DomControlThread::getAllDomainList()
{
    Result result;
    QStringList domainList;
    if ( currWorkConnection!=NULL && keep_alive ) {
        virDomainPtr *domains = NULL;
        unsigned int flags = VIR_CONNECT_LIST_DOMAINS_ACTIVE |
                             VIR_CONNECT_LIST_DOMAINS_INACTIVE;
        // the number of domains found or -1 and sets domains to NULL in case of error.
        int ret = virConnectListAllDomains(currWorkConnection, &domains, flags);
        if ( ret<0 ) {
            sendConnErrors();
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
            currentAttr<< QString().fromUtf8( virDomainGetName(domains[i]) )
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
        free(domains);
    };
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
        QString msg = QString("File \"%1\"\nnot opened.").arg(path);
        emit errorMsg( msg );
        return result;
    };
    xmlData = f.readAll();
    f.close();
    virDomainPtr domain = virDomainCreateXML(currWorkConnection, xmlData.data(), VIR_DOMAIN_START_AUTODESTROY);
    if ( domain==NULL ) {
        sendConnErrors();
        return result;
    };
    result.name = QString().fromUtf8( virDomainGetName(domain) );
    result.result = true;
    result.msg.append(QString("'<b>%1</b>' Domain from\n\"%2\"\nis created.")
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
        QString msg = QString("File \"%1\"\nnot opened.").arg(path);
        emit errorMsg( msg );
        return result;
    };
    xmlData = f.readAll();
    f.close();
    virDomainPtr domain = virDomainDefineXML(currWorkConnection, xmlData.data());
    sendConnErrors();
    if ( domain==NULL ) return result;
    result.name = QString().fromUtf8( virDomainGetName(domain) );
    result.result = true;
    result.msg.append(QString("'<b>%1</b>' Domain from\n\"%2\"\nis defined.")
                  .arg(result.name).arg(path));
    virDomainFree(domain);
    return result;
}
Result DomControlThread::startDomain()
{
    Result result;
    QString name = args.first();
    bool started = false;
    virDomainPtr domain = virDomainLookupByName(currWorkConnection, name.toUtf8().data());
    if ( domain!=NULL ) {
        started = (virDomainCreate(domain)+1) ? true : false;
        if (!started) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.name = name;
    result.result = started;
    result.msg.append(QString("'<b>%1</b>' Domain %2 Started.")
                      .arg(name).arg((started)?"":"don't"));
    return result;
}
Result DomControlThread::pauseDomain()
{
    Result result;
    QString name = args.first();
    QString state = args.last();
    bool invoked = false;

    virDomainPtr domain = virDomainLookupByName(currWorkConnection, name.toUtf8().data());

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
    result.msg.append(QString("'<b>%1</b>' Domain state %2 changed.")
                      .arg(name).arg((invoked)?"":"don't"));
    return result;
}
Result DomControlThread::destroyDomain()
{
    Result result;
    QString name = args.first();

    bool deleted = false;
    virDomainPtr domain = virDomainLookupByName(currWorkConnection, name.toUtf8().data());
    if ( domain!=NULL ) {
        deleted = (virDomainDestroy(domain)+1) ? true : false;
        if (!deleted) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.name = name;
    result.result = deleted;
    result.msg.append(QString("'<b>%1</b>' Domain %2 Destroyed.")
                      .arg(name).arg((deleted)?"":"don't"));
    return result;
}
Result DomControlThread::resetDomain()
{
    Result result;
    QString name = args.first();
    bool invoked = false;
    // extra flags; not used yet, so callers should always pass 0
    unsigned int flags = 0;

    virDomainPtr domain = virDomainLookupByName(currWorkConnection, name.toUtf8().data());

    if ( domain!=NULL ) {
        invoked = (virDomainReset(domain, flags)+1) ? true : false;
        if (!invoked) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.name = name;
    result.result = invoked;
    result.msg.append(QString("'<b>%1</b>' Domain %2 reset.")
                      .arg(name).arg((invoked)?"":"don't"));
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

    virDomainPtr domain = virDomainLookupByName(currWorkConnection, name.toUtf8().data());

    if ( domain!=NULL ) {
        invoked = (virDomainReboot(domain, flags)+1) ? true : false;
        if (!invoked) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.name = name;
    result.result = invoked;
    result.msg.append(QString("'<b>%1</b>' Domain %2 reboot.")
                      .arg(name).arg((invoked)?"":"don't"));
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

    virDomainPtr domain = virDomainLookupByName(currWorkConnection, name.toUtf8().data());

    if ( domain!=NULL ) {
        invoked = (virDomainShutdownFlags(domain, flags)+1) ? true : false;
        if (!invoked) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.name = name;
    result.result = invoked;
    result.msg.append(QString("'<b>%1</b>' Domain %2 shutdown.")
                      .arg(name).arg((invoked)?"":"don't"));
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

    virDomainPtr domain = virDomainLookupByName(currWorkConnection, name.toUtf8().data());

    if ( domain!=NULL ) {
        invoked = (virDomainSaveFlags(domain, to, dxml, flags)+1) ? true : false;
        if (!invoked) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.name = name;
    result.result = invoked;
    result.msg.append(QString("'<b>%1</b>' Domain %2 saved.")
                      .arg(name).arg((invoked)?"":"don't"));
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

    if ( currWorkConnection!=NULL ) {
        invoked = (virDomainRestoreFlags(currWorkConnection, from, dxml, flags)+1) ? true : false;
        if (!invoked) sendConnErrors();
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
    QString name = args.first();

    bool deleted = false;
    virDomainPtr domain = virDomainLookupByName(currWorkConnection, name.toUtf8().data());
    if ( domain!=NULL ) {
        deleted = (virDomainUndefine(domain)+1) ? true : false;
        if (!deleted) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.name = name;
    result.result = deleted;
    result.msg.append(QString("'<b>%1</b>' Domain %2 Undefined.")
                      .arg(name).arg((deleted)?"":"don't"));
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
    virDomainPtr domain = virDomainLookupByName(currWorkConnection, name.toUtf8().data());
    if ( domain!=NULL ) {
        set = (virDomainSetAutostart(domain, autostart)+1) ? true : false;
        if (!set) sendConnErrors();
        virDomainFree(domain);
    } else sendConnErrors();
    result.name = name;
    result.result = set;
    result.msg.append(QString("'<b>%1</b>' Domain autostart %2 Set.")
                      .arg(name).arg((set)?"":"don't"));
    return result;
}
Result DomControlThread::getDomainXMLDesc()
{
    Result result;
    QString name = args.first();

    bool read = false;
    char *Returns = NULL;
    virDomainPtr domain = virDomainLookupByName(currWorkConnection, name.toUtf8().data());
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
    if ( Returns!=NULL ) free(Returns);
    result.name = name;
    result.result = read;
    result.msg.append(QString("'<b>%1</b>' Domain %2 XML'ed")
                      .arg(name).arg((read)?"":"don't"));
    return result;
}
Result DomControlThread::migrateDomain()
{
    Result result;
    bool migrated = false;
    unsigned int flags = 0;
    qDebug()<<args<<"migrate";
    //qDebug()<<virConnectGetCapabilities(currWorkConnection);
    if ( args.count()<17 ) {
        result.name = args[0];
        result.result = migrated;
        result.msg.append(QString("'<b>%1</b>' Domain don't Migrated: %2")
                          .arg(args[0]).arg("arguments incorrect."));
        return result;
    };
    virDomainPtr domain = virDomainLookupByName(currWorkConnection, args[0].toUtf8().constData());
    //const char *uri = ( args[2].isEmpty() ) ? NULL : args[2].toUtf8().constData();
    bool ok;
    int maxDownTime = args[3].toInt(&ok);
    if (!ok) maxDownTime = 0;
    int bdw = args[4].toInt(&ok);
    if (!ok) bdw = 0;
    bool live = !args[5].isEmpty();
    bool p2p = !args[6].isEmpty();
    bool tun = !args[7].isEmpty();
    bool persist = !args[8].isEmpty();
    bool undefine = !args[9].isEmpty();
    bool pause = !args[10].isEmpty();
    bool full = !args[11].isEmpty();
    bool inc = !args[12].isEmpty();
    bool unsafe = !args[13].isEmpty();
    bool offline = !args[14].isEmpty();
    bool compressed = !args[15].isEmpty();
    bool abortOn = !args[16].isEmpty();
    //const char *dName = ( args[17].isEmpty() )? NULL : args[17].toUtf8().constData();
    //qDebug()<<args[0].toUtf8().constData()<<tun<< unsafe<<maxDownTime<<uri<<bdw<<args[17].toUtf8().constData();
    if ( domain!=NULL ) {
        // virDomainMigrateSetMaxDowntime
        // flags: extra flags; not used yet, so callers should always pass 0
        virDomainMigrateSetMaxDowntime(domain, maxDownTime, 0);
        if ( live )
            flags |= VIR_MIGRATE_LIVE;
        if ( p2p )
            flags |= VIR_MIGRATE_PEER2PEER;
        if ( tun )
            flags |= VIR_MIGRATE_TUNNELLED;
        if ( persist )
            flags |= VIR_MIGRATE_PERSIST_DEST;
        if ( undefine )
            flags |= VIR_MIGRATE_UNDEFINE_SOURCE;
        if ( pause )
            flags |= VIR_MIGRATE_PAUSED;
        if ( full )
            flags |= VIR_MIGRATE_NON_SHARED_DISK;
        if ( inc )
            flags |= VIR_MIGRATE_NON_SHARED_INC;
        if ( unsafe )
            flags |= VIR_MIGRATE_UNSAFE;
        if ( offline )
            flags |= VIR_MIGRATE_OFFLINE;
        if ( compressed )
            flags |= VIR_MIGRATE_COMPRESSED;
        if ( abortOn )
            flags |= VIR_MIGRATE_ABORT_ON_ERROR;
        if ( NULL!=destConnect ) {
            qDebug()<<"migrate to exist connect";
            virDomainPtr newDomain =
            virDomainMigrate(domain,
                             destConnect,
                             flags,
                             args[17].toUtf8().constData(),
                             args[2].toUtf8().constData(),
                             bdw);
            migrated = NULL!=newDomain;
            if (migrated) virDomainFree( newDomain );
        } else {
            qDebug()<< "migrate to URI";
            migrated = (virDomainMigrateToURI(domain,
                                              args[2].toUtf8().constData(),
                                              flags,
                                              args[17].toUtf8().constData(),
                                              bdw)
                        +1)?true:false;
        };
        virDomainFree(domain);
        if ( !migrated ) sendConnErrors();
    } else sendConnErrors();
    result.name = args[0];
    result.result = migrated;
    result.msg.append(QString("'<b>%1</b>' Domain %2 Migrated.")
                      .arg(args[0]).arg((migrated)?"":"don't"));
    if ( destConnect ) destConnect = NULL;
    return result;
}
Result DomControlThread::createSnapshoteDomain()
{
    Result result;
    bool snapped = false;
    if ( args.count()>2 ) {
        QString domName = args.at(0);
        result.name = args.at(0);
        unsigned int flags = args.at(1).toUInt();
        QByteArray _xmlDesc;
        _xmlDesc.append(args.at(2));
        const char *xmlDesc = _xmlDesc.data();
        //qDebug()<<xmlDesc<<flags;
        virDomainPtr domain = virDomainLookupByName(currWorkConnection, domName.toUtf8().data());
        if ( NULL==domain ) {
            sendConnErrors();
        } else {
            virDomainSnapshotPtr snapshot =
                    virDomainSnapshotCreateXML(domain, xmlDesc, flags);
            if ( NULL==snapshot ) {
                sendConnErrors();
            } else {
                snapped = true;
                virDomainSnapshotFree(snapshot);
            };
            virDomainFree(domain);
        };
    } else
        result.name = "error";
    result.result = snapped;
    result.msg.append(QString("'<b>%1</b>' Domain %2 snapped.")
                      .arg(result.name).arg((snapped)?"":"don't"));
    return result;
}
Result DomControlThread::revertSnapshoteDomain()
{
    Result result;
    bool reverted = false;
    if ( args.count()>2 ) {
        QString domName = args.at(0);
        result.name = args.at(0);
        QString snapshotName(args.at(1));
        unsigned int flags = args.at(2).toUInt();
        //qDebug()<<snapshotName<<flags;
        virDomainPtr domain = virDomainLookupByName(currWorkConnection, domName.toUtf8().data());
        if ( NULL==domain ) {
            sendConnErrors();
        } else {
            // flags: extra flags; not used yet, so callers should always pass 0
            virDomainSnapshotPtr snapshot =
                    virDomainSnapshotLookupByName(domain, snapshotName.toUtf8().data(), 0);
            if ( NULL==snapshot ) {
                sendConnErrors();
            } else {
                int ret = virDomainRevertToSnapshot(snapshot, flags);
                if ( ret<0 ) {
                    sendConnErrors();
                } else {
                    reverted = true;
                };
                virDomainSnapshotFree(snapshot);
            };
            virDomainFree(domain);
        };
    } else
        result.name = "error";
    result.msg.append(QString("'<b>%1</b>' Domain %2 reverted.")
                      .arg(result.name).arg((reverted)?"":"don't"));
    result.result = reverted;
    return result;
}
Result DomControlThread::deleteSnapshoteDomain()
{
    Result result;
    bool deleted = false;
    if ( args.count()>2 ) {
        QString domName = args.at(0);
        result.name = args.at(0);
        QString snapshotName(args.at(1));
        unsigned int flags = args.at(2).toUInt();
        //qDebug()<<snapshotName<<flags;
        virDomainPtr domain = virDomainLookupByName(currWorkConnection, domName.toUtf8().data());
        if ( NULL==domain ) {
            sendConnErrors();
        } else {
            // flags: extra flags; not used yet, so callers should always pass 0
            virDomainSnapshotPtr snapshot =
                    virDomainSnapshotLookupByName(domain, snapshotName.toUtf8().data(), 0);
            if ( NULL==snapshot ) {
                sendConnErrors();
            } else {
                int ret = virDomainSnapshotDelete(snapshot, flags);
                if ( ret<0 ) {
                    sendConnErrors();
                } else {
                    deleted = true;
                };
                virDomainSnapshotFree(snapshot);
            };
            virDomainFree(domain);
        };
    } else
        result.name = "error";
    result.msg.append(QString("'<b>%1</b>' Domain %2 deleted.")
                      .arg(result.name).arg((deleted)?"":"don't"));
    result.result = deleted;
    return result;
}
