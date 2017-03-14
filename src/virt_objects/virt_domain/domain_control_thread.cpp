#include "domain_control_thread.h"
#include <QDomDocument>

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
    if ( nullptr!=task.srcConnPtr ) {
        // for new virConnect usage create the new virConnectRef[erence]
        int ret = virConnectRef(*task.srcConnPtr);
        if ( ret<0 ) {
            task.srcConnPtr = nullptr;
            sendConnErrors();
        } else
            keep_alive = true;
    };
    if ( keep_alive && !isRunning() ) {
        ptr_ConnPtr = task.srcConnPtr;
        start();
    } else {
        Result result;
        result.type   = VIRT_ENTITY::VIRT_DOMAIN;
        result.number = number;
        result.action = Actions::_NONE_ACTION;
        emit resultData(result);
    };
}

/* private slots */
void DomControlThread::run()
{
    Result result;
    switch (task.action) {
    case Actions::GET_ALL_ENTITY_STATE :
        result = getAllDomainList();
        break;
    case Actions::GET_ALL_ENTITY_DATA0 :
        result = getDomainData0();
        break;
    case Actions::GET_ALL_ENTITY_DATA1 :
        result = getDomainData1();
        break;
    case Actions::CREATE_ENTITY :
        result = createDomain();
        break;
    case Actions::DEFINE_ENTITY :
        result = defineDomain();
        break;
    case Actions::EDIT_ENTITY :
        result = getDomainXMLDesc();
        break;
    case Actions::START_ENTITY :
        result = startDomain();
        break;
    case Actions::PAUSE_ENTITY :
        result = pauseDomain();
        break;
    case Actions::DESTROY_ENTITY :
        result = destroyDomain();
        break;
    case Actions::RESET_ENTITY :
        result = resetDomain();
        break;
    case Actions::REBOOT_ENTITY :
        result = rebootDomain();
        break;
    case Actions::SHUTDOWN_ENTITY :
        result = shutdownDomain();
        break;
    case Actions::SAVE_ENTITY :
        result = saveDomain();
        break;
    case Actions::RESTORE_ENTITY :
        result = restoreDomain();
        break;
    case Actions::UNDEFINE_ENTITY :
        result = undefineDomain();
        break;
    case Actions::CHANGE_ENTITY_AUTOSTART :
        result = changeAutoStartDomain();
        break;
    case Actions::GET_XML_DESCRIPTION :
        result = getDomainXMLDesc();
        break;
    case Actions::MIGRATE_ENTITY :
        result = migrateDomain();
        break;
    case Actions::CREATE_DOMAIN_SNAPSHOT :
        result = createSnapshoteDomain();
        break;
    case Actions::REVERT_TO_DOMAIN_SNAPSHOT :
        result = revertSnapshoteDomain();
        break;
    case Actions::DELETE_DOMAIN_SNAPSHOT :
        result = deleteSnapshoteDomain();
        break;
    default:
        break;
    };
    // task.srcConnPtr reference will closed in destructor as ptr_ConnPtr
    //virConnectClose(*task.srcConnPtr);
    result.type   = VIRT_ENTITY::VIRT_DOMAIN;
    result.number = number;
    result.action = task.action;
    emit resultData(result);
}
Result DomControlThread::getAllDomainList()
{
    Result result;
    ACT_RESULT domainList;
    if ( task.srcConnPtr!=nullptr && keep_alive ) {
        virDomainPtr *domains = nullptr;
        unsigned int flags =
                VIR_CONNECT_LIST_DOMAINS_ACTIVE |
                VIR_CONNECT_LIST_DOMAINS_INACTIVE;
        // the number of domains found or -1 and sets domains
        // to NULL in case of error.
        int ret = virConnectListAllDomains(
                    *task.srcConnPtr, &domains, flags);
        if ( ret<0 ) {
            result.err = sendConnErrors();
            result.result = false;
            result.data = domainList;
            return result;
        };

        // therefore correctly to use for() command,
        // because domains[0] can not exist.
        for (int i = 0; i < ret; i++) {
            QVariantMap currentAttr;
            int is_autostart = 0;
            virDomainGetAutostart(domains[i], &is_autostart);
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
            currentAttr.insert(
                        "name",
                        QString::fromUtf8( virDomainGetName(domains[i]) ));
            currentAttr.insert(
                        "active",
                        (virDomainIsActive(domains[i]))? true : false );
            currentAttr.insert(
                        "state", domainState);
            currentAttr.insert(
                        "auto", (is_autostart) ? true : false);
            currentAttr.insert(
                        "persistent",
                        (virDomainIsPersistent(domains[i]))? true : false );
            domainList.append(currentAttr);
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
    result.data   = domainList;
    return result;
}
Result DomControlThread::getDomainData0()
{
    Result result;
    QString activeDomainXmlDesc, displayType;
    QVariantMap domainDesc;
    QString name = task.object;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
    const char *_type = virConnectGetType(*task.srcConnPtr);
    result.name   = name;
    if ( _type==nullptr ) {
        result.result = false;
        result.err = "Error in getting the connection type.";
    } else {
        result.result = true;
    };
    virDomainPtr domainPtr =virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    // flag=0 for get running domain xml-description
    activeDomainXmlDesc.append( virDomainGetXMLDesc(domainPtr, 0) );
    QDomDocument doc;
    doc.setContent(activeDomainXmlDesc);
    QDomElement graph = doc.firstChildElement("domain")
       .firstChildElement("devices")
       .firstChildElement("graphics");
    if ( !graph.isNull() )
        displayType.append( graph.attribute("type").toLower() );
    domainDesc.insert("DomainType", _type);
    domainDesc.insert("DisplayType", displayType);
    result.data.append(domainDesc);
    return result;
}
Result DomControlThread::getDomainData1()
{
    Result result;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
    QString activeDomainXmlDesc, url, uri, type, addr, port;
    unsigned int flags = 0;
    virDomainPtr domainPtr =virDomainLookupByName(
                *task.srcConnPtr, task.object.toUtf8().data());
    // flag=0 for get running domain xml-description
    activeDomainXmlDesc.append( virDomainGetXMLDesc(domainPtr, flags) );
    QDomDocument doc;
    doc.setContent(activeDomainXmlDesc);
    QDomElement graph = doc.firstChildElement("domain")
       .firstChildElement("devices")
       .firstChildElement("graphics");
    uri.append(virConnectGetURI(*task.srcConnPtr));
    addr = uri.split("://").last();
    uri  = addr;
    addr = uri.split("/").first();
    uri  = addr;
    addr = uri.split(":").first();
    uri  = addr;
    if ( uri.contains("@") ) {
        addr = uri.split("@").last();
        uri  = addr;
    };
    addr.clear();
    if ( !graph.isNull() ) {
        type.append( graph.attribute("type").toLower() );
        if (graph.hasAttribute("listen")) {
            // for listen address
            addr = graph.attribute("listen");
        } else if ( !graph.firstChildElement("listen").isNull() ) {
            // for listen address from virt.network
            addr = graph.firstChildElement("listen")
                    .attribute("address");
        } else {
            // if uri is empty then connection is local
            if ( uri.isEmpty() ) {
                addr = "127.0.0.1";
                uri  = "127.0.0.1";
            };
        };
        port = (graph.hasAttribute("port"))?
                    graph.attribute("port") : "5900";
    } else {
        type = "???";
        addr = "???";
        port = "???";
    };
    // if connection not local then use address from uri
    if ( addr!=uri && !uri.isEmpty() ) addr = uri;
    url = QString("%1://%2:%3").arg(type).arg(addr).arg(port);
    QVariantMap _url;
    _url.insert("URL", url);
    result.data.append( _url );
    result.result = true;
    return result;
}
Result DomControlThread::createDomain()
{
    Result result;
    QString path = task.args.path;
    QByteArray xmlData;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
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
    if ( domain==nullptr ) {
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
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
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
    if ( domain==nullptr ) {
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
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
    bool started = false;
    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( domain!=nullptr ) {
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
    bool invoked = false;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };

    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());

    if ( domain!=nullptr ) {
        int state;
        int reason;
        // flags : extra flags; not used yet, so callers should always pass 0
        unsigned int flags = 0;
        if ( virDomainGetState(domain, &state, &reason, flags)+1 ) {
            switch (state) {
            case VIR_DOMAIN_RUNNING:
                invoked = (virDomainSuspend(domain)+1) ? true : false;
                if (!invoked)
                    result.err = sendConnErrors();
                break;
            case VIR_DOMAIN_PAUSED:
                invoked = (virDomainResume(domain)+1) ? true : false;
                if (!invoked)
                    result.err = sendConnErrors();
                break;
            default:
                break;
            virDomainFree(domain);
            };
        };
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
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( domain!=nullptr ) {
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
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };

    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());

    if ( domain!=nullptr ) {
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
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };

    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());

    if ( domain!=nullptr ) {
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
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };

    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());

    if ( domain!=nullptr ) {
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
    const char *dxml = nullptr;
    bool invoked = false;
    unsigned int flags = VIR_DOMAIN_SAVE_BYPASS_CACHE;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };

    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());

    if ( domain!=nullptr ) {
        int state;
        int reason;
        // flags : extra flags; not used yet, so callers should always pass 0
        unsigned int st_flags = 0;
        if ( virDomainGetState(domain, &state, &reason, st_flags)+1 ) {
            switch (state) {
            case VIR_DOMAIN_RUNNING:
                flags = flags | VIR_DOMAIN_SAVE_RUNNING;
                break;
                break;
            case VIR_DOMAIN_PAUSED:
                flags = flags | VIR_DOMAIN_SAVE_PAUSED;
                break;
            default:
                break;
            };
        };
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
    const char *dxml = nullptr;
    QString to_state = task.args.state;
    bool invoked = false;
    unsigned int flags = VIR_DOMAIN_SAVE_BYPASS_CACHE;
    if ( to_state=="RUNNING" ) {
        flags = flags | VIR_DOMAIN_SAVE_RUNNING;
    } else if ( to_state=="PAUSED" ) {
        flags = flags | VIR_DOMAIN_SAVE_PAUSED;
    };

    if ( task.srcConnPtr!=nullptr ) {
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
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( domain!=nullptr ) {
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
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };

    bool set = false;
    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( domain!=nullptr ) {
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
    char *Returns = nullptr;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( domain!=nullptr ) {
        Returns = (virDomainGetXMLDesc(domain, flags));
        if ( Returns==nullptr )
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
    if ( Returns!=nullptr ) free(Returns);
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
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, result.name.toUtf8().constData());
    //const char *uri = ( args[2].isEmpty() ) ? nullptr : args[2].toUtf8().constData();
    int maxDownTime = task.args.offset;
    int bdw = task.args.size;
    if ( domain!=nullptr ) {
        // virDomainMigrateSetMaxDowntime
        // flags: extra flags; not used yet, so callers should always pass 0
        virDomainMigrateSetMaxDowntime(domain, maxDownTime, 0);
        if ( nullptr!=task.args.dstConnPtr ) {
            qDebug()<<"migrate to exist connect";
            virDomainPtr newDomain =
            virDomainMigrate(domain,
                             *task.args.dstConnPtr,
                             flags,
                             task.args.object.toUtf8().data(),
                             task.args.path.toUtf8().data(),
                             bdw);
            migrated = nullptr!=newDomain;
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
    //if ( task.args.dstConnPtr ) task.args.dstConnPtr = nullptr;
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
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, domName.toUtf8().data());
    if ( nullptr==domain ) {
        result.err = sendConnErrors();
    } else {
        virDomainSnapshotPtr snapshot =
                virDomainSnapshotCreateXML(domain, xmlDesc, flags);
        if ( nullptr==snapshot ) {
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
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, domName.toUtf8().data());
    if ( nullptr==domain ) {
        result.err = sendConnErrors();
    } else {
        // flags: extra flags; not used yet, so callers should always pass 0
        virDomainSnapshotPtr snapshot = virDomainSnapshotLookupByName(
                    domain, snapshotName.toUtf8().data(), 0);
        if ( nullptr==snapshot ) {
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
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
    virDomainPtr domain = virDomainLookupByName(
                *task.srcConnPtr, domName.toUtf8().data());
    if ( nullptr==domain ) {
        result.err = sendConnErrors();
    } else {
        // flags: extra flags; not used yet, so callers should always pass 0
        virDomainSnapshotPtr snapshot =
                virDomainSnapshotLookupByName(
                    domain, snapshotName.toUtf8().data(), 0);
        if ( nullptr==snapshot ) {
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
