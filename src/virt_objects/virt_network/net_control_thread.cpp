#include "net_control_thread.h"

NetControlThread::NetControlThread(QObject *parent) :
    ControlThread(parent)
{
}

/* public slots */
void NetControlThread::execAction(uint _num, TASK _task)
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
        result.type   = VIRT_ENTITY::VIRT_NETWORK;
        result.number = number;
        result.action = Actions::_NONE_ACTION;
        emit resultData(result);
    };
}

/* private slots */
void NetControlThread::run()
{
    Result result;
    switch (task.action) {
    case Actions::GET_ALL_ENTITY_STATE :
        result = getAllNetworkList();
        break;
    case Actions::CREATE_ENTITY :
        result = createNetwork();
        break;
    case Actions::DEFINE_ENTITY :
        result = defineNetwork();
        break;
    case Actions::EDIT_ENTITY :
        result = getVirtNetXMLDesc();
        break;
    case Actions::START_ENTITY :
        result = startNetwork();
        break;
    case Actions::DESTROY_ENTITY :
        result = destroyNetwork();
        break;
    case Actions::UNDEFINE_ENTITY :
        result = undefineNetwork();
        break;
    case Actions::CHANGE_ENTITY_AUTOSTART :
        result = changeAutoStartNetwork();
        break;
    case Actions::GET_XML_DESCRIPTION :
        result = getVirtNetXMLDesc();
        break;
    default:
        break;
    };
    // task.srcConnPtr reference will closed in destructor as ptr_ConnPtr
    //virConnectClose(*task.srcConnPtr);
    result.type   = VIRT_ENTITY::VIRT_NETWORK;
    result.number = number;
    result.action = task.action;
    emit resultData(result);
}
Result NetControlThread::getAllNetworkList()
{
    Result result;
    ACT_RESULT virtNetList;
    if ( task.srcConnPtr!=nullptr && keep_alive ) {
        virNetworkPtr *networks = nullptr;
        unsigned int flags =
                VIR_CONNECT_LIST_NETWORKS_ACTIVE |
                VIR_CONNECT_LIST_NETWORKS_INACTIVE;
        int ret = virConnectListAllNetworks(
                    *task.srcConnPtr, &networks, flags);
        if ( ret<0 ) {
            result.err = sendConnErrors();
            return result;
        };

        // therefore correctly to use for() command,
        // because networks[0] can not exist.
        for (int i = 0; i < ret; i++) {
            QVariantMap currentAttr;
            int is_autostart = 0;
            virNetworkGetAutostart(networks[i], &is_autostart);
            currentAttr.insert(
                        "name",
                        QString::fromUtf8( virNetworkGetName(networks[i]) ));
            currentAttr.insert(
                        "active",
                        (virNetworkIsActive(networks[i]))
                        ? true : false );
            currentAttr.insert(
                        "auto", (is_autostart)? true : false);
            currentAttr.insert(
                        "persistent",
                        (virNetworkIsPersistent(networks[i]))
                        ? true : false );
            virtNetList.append(currentAttr);
            //qDebug()<<currentAttr;
            virNetworkFree(networks[i]);
        };
        if (networks) free(networks);
    };
    result.result = true;
    result.data   = virtNetList;
    return result;
}
Result NetControlThread::createNetwork()
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
    virNetworkPtr network = virNetworkCreateXML(
                *task.srcConnPtr, xmlData.data());
    if ( network==nullptr ) {
        result.err = sendConnErrors();
        return result;
    };
    result.name = QString::fromUtf8( virNetworkGetName(network) );
    result.result = true;
    result.msg.append(QString("'<b>%1</b>' Network from\n\"%2\"\nis created.")
                      .arg(result.name).arg(path));
    virNetworkFree(network);
    return result;
}
Result NetControlThread::defineNetwork()
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
    virNetworkPtr network = virNetworkDefineXML(
                *task.srcConnPtr, xmlData.data());
    if ( network==nullptr ) {
        result.err = sendConnErrors();
        return result;
    };
    result.name = QString::fromUtf8( virNetworkGetName(network) );
    result.result = true;
    result.msg.append(QString("'<b>%1</b>' Network from\n\"%2\"\nis defined.")
                      .arg(result.name).arg(path));
    virNetworkFree(network);
    return result;
}
Result NetControlThread::startNetwork()
{
    Result result;
    QString name = task.object;
    bool started = false;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
    virNetworkPtr network = virNetworkLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( network!=nullptr ) {
        started = (virNetworkCreate(network)+1) ? true : false;
        if (!started)
            result.err = sendConnErrors();
        virNetworkFree(network);
    } else
        result.err = sendConnErrors();
    result.name = name;
    result.result = started;
    result.msg.append(QString("'<b>%1</b>' Network %2 Started.")
                      .arg(name).arg((started)?"":"don't"));
    return result;
}
Result NetControlThread::destroyNetwork()
{
    Result result;
    QString name = task.object;
    bool deleted = false;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
    virNetworkPtr network = virNetworkLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( network!=nullptr ) {
        deleted = (virNetworkDestroy(network)+1) ? true : false;
        if (!deleted)
            result.err = sendConnErrors();
        virNetworkFree(network);
    } else
        result.err = sendConnErrors();
    result.name = name;
    result.result = deleted;
    result.msg.append(QString("'<b>%1</b>' Network %2 Destroyed.")
                      .arg(name).arg((deleted)?"":"don't"));
    return result;
}
Result NetControlThread::undefineNetwork()
{
    Result result;
    QString name = task.object;
    bool deleted = false;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
    virNetworkPtr network = virNetworkLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( network!=nullptr ) {
        deleted = (virNetworkUndefine(network)+1) ? true : false;
        if (!deleted)
            result.err = sendConnErrors();
        virNetworkFree(network);
    } else
        result.err = sendConnErrors();
    result.name = name;
    result.result = deleted;
    result.msg.append(QString("'<b>%1</b>' Network %2 Undefined.")
                      .arg(name).arg((deleted)?"":"don't"));
    return result;
}
Result NetControlThread::changeAutoStartNetwork()
{
    Result result;
    QString name = task.object;
    result.name = name;
    int autostart = task.args.sign;
    bool set = false;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
    virNetworkPtr network = virNetworkLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( network!=nullptr ) {
        set = (virNetworkSetAutostart(network, autostart)+1) ? true : false;
        if (!set)
            result.err = sendConnErrors();
        virNetworkFree(network);
    } else
        result.err = sendConnErrors();
    result.result = set;
    result.msg.append(QString("'<b>%1</b>' Network autostart %2 Set.")
                      .arg(name).arg((set)?"":"don't"));
    return result;
}
Result NetControlThread::getVirtNetXMLDesc()
{
    Result result;
    QString name = task.object;
    result.name = name;
    bool read = false;
    char *Returns = nullptr;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
    virNetworkPtr network = virNetworkLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( network!=nullptr ) {
        Returns = virNetworkGetXMLDesc(
                    network, VIR_NETWORK_XML_INACTIVE);
        if ( Returns==nullptr )
            result.err = sendConnErrors();
        else read = true;
        virNetworkFree(network);
    } else
        result.err = sendConnErrors();
    QTemporaryFile f;
    f.setAutoRemove(false);
    f.setFileTemplate(
                QString("%1%2XML_Desc-XXXXXX.xml")
                .arg(QDir::tempPath()).arg(QDir::separator()));
    read = f.open();
    if (read) f.write(Returns);
    result.fileName.append(f.fileName());
    f.close();
    if ( Returns!=nullptr ) free(Returns);
    result.result = read;
    result.msg.append(QString("'<b>%1</b>' Network %2 XML'ed")
                      .arg(name).arg((read)?"":"don't"));
    return result;
}
