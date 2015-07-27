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
    keep_alive = true;
    // for new virConnect usage create the new virConnectRef[erence]
    int ret = virConnectRef(task.sourceConn);
    if ( ret<0 ) {
        task.sourceConn = NULL;
        sendConnErrors();
        keep_alive = false;
    };
    if ( keep_alive && !isRunning() ) {
        if ( NULL!=task.sourceConn ) start();
        else {
            Result result;
            result.type   = "iface";
            result.number = number;
            result.action = _EMPTY_ACTION;
            emit resultData(result);
        };
    };
}

/* private slots */
void InterfaceControlThread::run()
{
    Result result;
    switch (task.action) {
    case GET_ALL_ENTITY :
        result = getAllIfaceList();
        break;
    case START_ENTITY :
        result = startIface();
        break;
    case DESTROY_ENTITY :
        result = destroyIface();
        break;
    case DEFINE_ENTITY :
        result = defineIface();
        break;
    case UNDEFINE_ENTITY :
        result = undefineIface();
        break;
    case IFACE_CHANGE_BEGIN :
        result = ifaceChangeBegin();
        break;
    case IFACE_CHANGE_COMMIT :
        result = ifaceChangeCommit();
        break;
    case IFACE_CHANGE_ROLLBACK :
        result = ifaceChangeRollback();
        break;
    case GET_XML_DESCRIPTION :
        result = getVirtIfaceXMLDesc();
        break;
    default:
        break;
    };
    virConnectClose(task.sourceConn);
    result.type   = "iface";
    result.number = number;
    result.action = task.action;
    emit resultData(result);
}
Result InterfaceControlThread::getAllIfaceList()
{
    Result result;
    QStringList virtIfaceList;
    if ( task.sourceConn!=NULL && keep_alive ) {
        virInterfacePtr *ifaces = NULL;
        //get all ifaces
        unsigned int flags =
                VIR_CONNECT_LIST_INTERFACES_INACTIVE |
                VIR_CONNECT_LIST_INTERFACES_ACTIVE;
        int ret = virConnectListAllInterfaces(task.sourceConn, &ifaces, flags);
        if ( ret<0 ) {
            sendConnErrors();
            return result;
        };

        // therefore correctly to use for() command, because ifaces[0] can not exist.
        for (int i = 0; i < ret; i++) {
            QStringList currentAttr;
            const char* name = virInterfaceGetName(ifaces[i]);
            const char* MAC = virInterfaceGetMACString(ifaces[i]);

            QString state = virInterfaceIsActive(ifaces[i]) ? "active" : "inactive";
            currentAttr<<QString::fromUtf8(name)\
                       <<QString::fromUtf8(MAC)\
                       <<state\
                       <<"";
            virtIfaceList.append(currentAttr.join(DFR));
            virInterfaceFree(ifaces[i]);
        };
        free(ifaces);
    };
    result.result = true;
    result.msg = virtIfaceList;
    return result;
}
Result InterfaceControlThread::startIface()
{
    Result result;
    QString name = task.object;
    bool started = false;
    virInterfacePtr iface = virInterfaceLookupByName(
                task.sourceConn, name.toUtf8().data());
    if ( iface!=NULL ) {
        // extra flags; not used yet, so callers should always pass 0
        started = (virInterfaceCreate(iface, 0)+1) ? true : false;
        if (!started) sendConnErrors();
        virInterfaceFree(iface);
    } else sendConnErrors();
    result.name = name;
    result.result = started;
    result.msg.append(
                QString("'<b>%1</b>' Interface %2 started.")
                .arg(name).arg((started)?"":"don't"));
    return result;
}
Result InterfaceControlThread::destroyIface()
{
    Result result;
    QString name = task.object;
    bool destroyed = false;
    virInterfacePtr iface = virInterfaceLookupByName(
                task.sourceConn, name.toUtf8().data());
    if ( iface!=NULL ) {
        // extra flags; not used yet, so callers should always pass 0
        destroyed = (virInterfaceDestroy(iface, 0)+1) ? true : false;
        if (!destroyed) sendConnErrors();
        virInterfaceFree(iface);
    } else sendConnErrors();
    result.name = name;
    result.result = destroyed;
    result.msg.append(
                QString("'<b>%1</b>' Interface %2 destroyed.")
                .arg(name).arg((destroyed)?"":"don't"));
    return result;
}
Result InterfaceControlThread::defineIface()
{
    Result result;
    QString path = task.args.path;
    QByteArray xmlData;
    QFile f;
    f.setFileName(path);
    if ( !f.open(QIODevice::ReadOnly) ) {
        QString msg = QString("File \"%1\"\nnot opened.").arg(path);
        emit errorMsg( msg, number );
        return result;
    };
    xmlData = f.readAll();
    f.close();
    //extra flags; not used yet, so callers should always pass 0
    int flags = 0;
    virInterfacePtr iface = virInterfaceDefineXML(
                task.sourceConn, xmlData.data(), flags);
    if ( iface==NULL ) {
        sendConnErrors();
        return result;
    };
    result.name = QString::fromUtf8(virInterfaceGetName(iface));
    result.result = true;
    result.msg.append(
                QString("'<b>%1</b>' Interface from\n\"%2\"\nis defined.")
                .arg(result.name).arg(path));
    virInterfaceFree(iface);
    return result;
}
Result InterfaceControlThread::undefineIface()
{
    Result result;
    QString name = task.object;
    bool deleted = false;
    virInterfacePtr iface = virInterfaceLookupByName(
                task.sourceConn, name.toUtf8().data());
    if ( iface!=NULL ) {
        deleted = (virInterfaceUndefine(iface)+1) ? true : false;
        if (!deleted) sendConnErrors();
        virInterfaceFree(iface);
    } else sendConnErrors();
    result.name = name;
    result.result = deleted;
    result.msg.append(
                QString("'<b>%1</b>' Interface %2 undefined.")
                .arg(name).arg((deleted)?"":"don't"));
    return result;
}
Result InterfaceControlThread::ifaceChangeBegin()
{
    Result result;
    QString name = task.object;
    bool processed = false;
    virInterfacePtr iface = virInterfaceLookupByName(
                task.sourceConn, name.toUtf8().data());
    if ( iface!=NULL ) {
        // extra flags; not used yet, so callers should always pass 0
        processed = (virInterfaceUndefine(iface)+1) ? true : false;
        if (!processed) sendConnErrors();
        virInterfaceFree(iface);
    } else sendConnErrors();
    result.name = name;
    result.result = processed;
    result.msg.append(
                QString("'<b>%1</b>' Interface %2 in changing.")
                .arg(name).arg((processed)?"":"don't"));
    return result;
}
Result InterfaceControlThread::ifaceChangeCommit()
{
    Result result;
    QString name = task.object;
    bool processed = false;
    virInterfacePtr iface = virInterfaceLookupByName(
                task.sourceConn, name.toUtf8().data());
    if ( iface!=NULL ) {
        // extra flags; not used yet, so callers should always pass 0
        processed = (virInterfaceUndefine(iface)+1) ? true : false;
        if (!processed) sendConnErrors();
        virInterfaceFree(iface);
    } else sendConnErrors();
    result.name = name;
    result.result = processed;
    result.msg.append(
                QString("'<b>%1</b>' Interface %2 commited.")
                .arg(name).arg((processed)?"":"don't"));
    return result;
}
Result InterfaceControlThread::ifaceChangeRollback()
{
    Result result;
    QString name = task.object;
    bool processed = false;
    virInterfacePtr iface = virInterfaceLookupByName(
                task.sourceConn, name.toUtf8().data());
    if ( iface!=NULL ) {
        // extra flags; not used yet, so callers should always pass 0
        processed = (virInterfaceUndefine(iface)+1) ? true : false;
        if (!processed) sendConnErrors();
        virInterfaceFree(iface);
    } else sendConnErrors();
    result.name = name;
    result.result = processed;
    result.msg.append(
                QString("'<b>%1</b>' Interface %2 rollbacked.")
                .arg(name).arg((processed)?"":"don't"));
    return result;
}
Result InterfaceControlThread::getVirtIfaceXMLDesc()
{
    Result result;
    QString name = task.object;
    result.name = name;
    bool read = false;
    char *Returns = NULL;
    virInterfacePtr iface = virInterfaceLookupByName(
                task.sourceConn, name.toUtf8().data());
    if ( iface!=NULL ) {
        //extra flags; not used yet, so callers should always pass 0
        int flags = 0;
        Returns = (virInterfaceGetXMLDesc(iface, flags));
        if ( Returns==NULL ) sendConnErrors();
        else read = true;
        virInterfaceFree(iface);
    } else sendConnErrors();
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
    result.result = read;
    result.msg.append(
                QString("'<b>%1</b>' Interface %2 XML'ed")
                .arg(name).arg((read)?"":"don't"));
    return result;
}
