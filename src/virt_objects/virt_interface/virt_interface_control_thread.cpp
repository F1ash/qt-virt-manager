#include "virt_interface_control_thread.h"

InterfaceControlThread::InterfaceControlThread(QObject *parent) :
    ControlThread(parent)
{
}

/* public slots */
void InterfaceControlThread::execAction(uint _num, TASK _task)
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
        result.type   = VIRT_ENTITY::VIRT_INTERFACE;
        result.number = number;
        result.action = Actions::_NONE_ACTION;
        emit resultData(result);
    };
}

/* private slots */
void InterfaceControlThread::run()
{
    Result result;
    switch (task.action) {
    case Actions::GET_ALL_ENTITY_STATE :
        result = getAllIfaceList();
        break;
    case Actions::START_ENTITY :
        result = startIface();
        break;
    case Actions::DESTROY_ENTITY :
        result = destroyIface();
        break;
    case Actions::DEFINE_ENTITY :
        result = defineIface();
        break;
    case Actions::UNDEFINE_ENTITY :
        result = undefineIface();
        break;
    case Actions::IFACE_CHANGE_BEGIN :
        result = ifaceChangeBegin();
        break;
    case Actions::IFACE_CHANGE_COMMIT :
        result = ifaceChangeCommit();
        break;
    case Actions::IFACE_CHANGE_ROLLBACK :
        result = ifaceChangeRollback();
        break;
    case Actions::GET_XML_DESCRIPTION :
        result = getVirtIfaceXMLDesc();
        break;
    default:
        break;
    };
    // task.srcConnPtr reference will closed in destructor as ptr_ConnPtr
    //virConnectClose(*task.srcConnPtr);
    result.type   = VIRT_ENTITY::VIRT_INTERFACE;
    result.number = number;
    result.action = task.action;
    emit resultData(result);
}
Result InterfaceControlThread::getAllIfaceList()
{
    Result result;
    ACT_RESULT virtIfaceList;
    if ( task.srcConnPtr!=nullptr && keep_alive ) {
        virInterfacePtr *ifaces = nullptr;
        //get all ifaces
        unsigned int flags =
                VIR_CONNECT_LIST_INTERFACES_INACTIVE |
                VIR_CONNECT_LIST_INTERFACES_ACTIVE;
        int ret = virConnectListAllInterfaces(
                    *task.srcConnPtr, &ifaces, flags);
        if ( ret<0 ) {
            result.err = sendConnErrors();
            return result;
        };

        // therefore correctly to use for() command,
        // because ifaces[0] can not exist.
        for (int i = 0; i < ret; i++) {
            QVariantMap currentAttr;
            const char* name = virInterfaceGetName(ifaces[i]);
            const char* MAC = virInterfaceGetMACString(ifaces[i]);

            currentAttr.insert(
                        "name", QString::fromUtf8(name));
            currentAttr.insert(
                        "MAC", QString::fromUtf8(MAC));
            currentAttr.insert(
                        "state",
                        virInterfaceIsActive(ifaces[i])
                        ? true : false);
            currentAttr.insert(
                        "changing", false);
            //qDebug()<<currentAttr;
            virtIfaceList.append(currentAttr);
            virInterfaceFree(ifaces[i]);
        };
        if (ifaces) free(ifaces);
    };
    result.result = true;
    result.data   = virtIfaceList;
    return result;
}
Result InterfaceControlThread::startIface()
{
    Result result;
    QString name = task.object;
    bool started = false;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    virInterfacePtr iface = virInterfaceLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( iface!=nullptr ) {
        // extra flags; not used yet, so callers should always pass 0
        started = (virInterfaceCreate(iface, 0)+1) ? true : false;
        if (!started)
            result.err = sendConnErrors();
        virInterfaceFree(iface);
    } else
        result.err = sendConnErrors();
    result.name = name;
    result.result = started;
    result.msg.append(
                QString(tr("'<b>%1</b>' Interface %2 started."))
                .arg(name).arg((started)? "": tr("don't")));
    return result;
}
Result InterfaceControlThread::destroyIface()
{
    Result result;
    QString name = task.object;
    bool destroyed = false;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    virInterfacePtr iface = virInterfaceLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( iface!=nullptr ) {
        // extra flags; not used yet, so callers should always pass 0
        destroyed = (virInterfaceDestroy(iface, 0)+1) ? true : false;
        if (!destroyed)
            result.err = sendConnErrors();
        virInterfaceFree(iface);
    } else
        result.err = sendConnErrors();
    result.name = name;
    result.result = destroyed;
    result.msg.append(
                QString(tr("'<b>%1</b>' Interface %2 destroyed."))
                .arg(name).arg((destroyed)? "": tr("don't")));
    return result;
}
Result InterfaceControlThread::defineIface()
{
    Result result;
    QString path = task.args.path;
    QByteArray xmlData;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    QFile f;
    f.setFileName(path);
    if ( !f.open(QIODevice::ReadOnly) ) {
        QString msg = QString(tr("File \"%1\"\nnot opened.")).arg(path);
        emit errorMsg( msg, number );
        result.err = msg;
        return result;
    };
    xmlData = f.readAll();
    f.close();
    //extra flags; not used yet, so callers should always pass 0
    int flags = 0;
    virInterfacePtr iface = virInterfaceDefineXML(
                *task.srcConnPtr, xmlData.data(), flags);
    if ( iface==nullptr ) {
        result.err = sendConnErrors();
        return result;
    };
    result.name = QString::fromUtf8(virInterfaceGetName(iface));
    result.result = true;
    result.msg.append(
                QString(tr("'<b>%1</b>' Interface from\n\"%2\"\nis defined."))
                .arg(result.name).arg(path));
    virInterfaceFree(iface);
    return result;
}
Result InterfaceControlThread::undefineIface()
{
    Result result;
    QString name = task.object;
    bool deleted = false;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    virInterfacePtr iface = virInterfaceLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( iface!=nullptr ) {
        deleted = (virInterfaceUndefine(iface)+1) ? true : false;
        if (!deleted)
            result.err = sendConnErrors();
        virInterfaceFree(iface);
    } else
        result.err = sendConnErrors();
    result.name = name;
    result.result = deleted;
    result.msg.append(
                QString(tr("'<b>%1</b>' Interface %2 undefined."))
                .arg(name).arg((deleted)? "": tr("don't")));
    return result;
}
Result InterfaceControlThread::ifaceChangeBegin()
{
    Result result;
    QString name = task.object;
    bool processed = false;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    // extra flags; not used yet, so callers should always pass 0
    processed = (virInterfaceChangeBegin(*task.srcConnPtr, 0)+1) ? true : false;
    if (!processed)
        result.err = sendConnErrors();

    result.name = name;
    result.result = processed;
    result.msg.append(
                QString(tr("'<b>%1</b>' Interface %2 in changing."))
                .arg(name).arg((processed)? "": tr("don't")));
    return result;
}
Result InterfaceControlThread::ifaceChangeCommit()
{
    Result result;
    QString name = task.object;
    bool processed = false;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    // extra flags; not used yet, so callers should always pass 0
    processed = (virInterfaceChangeCommit(*task.srcConnPtr, 0)+1) ? true : false;
    if (!processed)
        result.err = sendConnErrors();

    result.name = name;
    result.result = processed;
    result.msg.append(
                QString(tr("'<b>%1</b>' Interface %2 commited."))
                .arg(name).arg((processed)? "": tr("don't")));
    return result;
}
Result InterfaceControlThread::ifaceChangeRollback()
{
    Result result;
    QString name = task.object;
    bool processed = false;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    // extra flags; not used yet, so callers should always pass 0
    processed = (virInterfaceChangeRollback(*task.srcConnPtr, 0)+1) ? true : false;
    if (!processed)
        result.err = sendConnErrors();

    result.name = name;
    result.result = processed;
    result.msg.append(
                QString(tr("'<b>%1</b>' Interface %2 rollbacked."))
                .arg(name).arg((processed)? "": tr("don't")));
    return result;
}
Result InterfaceControlThread::getVirtIfaceXMLDesc()
{
    Result result;
    QString name = task.object;
    result.name = name;
    bool read = false;
    char *Returns = nullptr;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = tr("Connection pointer is NULL.");
        return result;
    };
    virInterfacePtr iface = virInterfaceLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( iface!=nullptr ) {
        //extra flags; not used yet, so callers should always pass 0
        int flags = 0;
        Returns = virInterfaceGetXMLDesc(iface, flags);
        if ( Returns==nullptr )
            result.err = sendConnErrors();
        else read = true;
        virInterfaceFree(iface);
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
    result.result = read;
    result.msg.append(
                QString(tr("'<b>%1</b>' Interface %2 XML'ed."))
                .arg(name).arg((read)? "": tr("don't")));
    return result;
}
