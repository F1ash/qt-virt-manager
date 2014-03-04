#include "net_control_thread.h"

NetControlThread::NetControlThread(QObject *parent) :
    QThread(parent)
{
    qRegisterMetaType<Actions>("Actions");
}

/* public slots */
bool NetControlThread::setCurrentWorkConnect(virConnectPtr conn)
{
    keep_alive = true;
    currWorkConnect = conn;
    //qDebug()<<"net_thread"<<currWorkConnect;
}
void NetControlThread::stop() { keep_alive = false; }
void NetControlThread::execAction(Actions act, QStringList _args)
{
    action = EMPTY_ACTION;
    args.clear();
    if ( keep_alive && !isRunning() ) {
        action = act;
        args = _args;
        start();
    };
}

/* private slots */
void NetControlThread::run()
{
    QStringList result;
    switch (action) {
    case GET_ALL_NETWORK :
        result.append(getAllNetworkList());
        break;
    case CREATE_NETWORK :
        result.append(createNetwork());
        break;
    case DEFINE_NETWORK :
        result.append(defineNetwork());
        break;
    case START_NETWORK :
        result.append(startNetwork());
        break;
    case DESTROY_NETWORK :
        result.append(destroyNetwork());
        break;
    case UNDEFINE_NETWORK :
        result.append(undefineNetwork());
        break;
    case CHANGE_AUTOSTART :
        result.append(changeAutoStartNetwork());
        break;
    default:
        break;
    };
    if ( !result.isEmpty() ) emit resultData(action, result);
}
QStringList NetControlThread::getAllNetworkList()
{
    QStringList virtNetList;
    if ( currWorkConnect!=NULL && keep_alive ) {
        virNetworkPtr *network;
        unsigned int flags = VIR_CONNECT_LIST_NETWORKS_ACTIVE |
                             VIR_CONNECT_LIST_NETWORKS_INACTIVE;
        int ret = virConnectListAllNetworks( currWorkConnect, &network, flags);
        if ( ret<0 ) {
            sendConnErrors();
            free(network);
            return virtNetList;
        };

        int i = 0;
        while ( network[i] != NULL ) {
            QStringList currentAttr;
            QString autostartStr;
            int is_autostart = 0;
            if (virNetworkGetAutostart(network[i], &is_autostart) < 0) {
                autostartStr.append("no autostart");
            } else autostartStr.append( is_autostart ? "yes" : "no" );
            currentAttr<< QString( virNetworkGetName(network[i]) )
                       << QString( virNetworkIsActive(network[i]) ? "active" : "inactive" )
                       << autostartStr
                       << QString( virNetworkIsPersistent(network[i]) ? "yes" : "no" );
            virtNetList.append(currentAttr.join(" "));
            //qDebug()<<currentAttr;
            virNetworkFree(network[i]);
            i++;
        };
        free(network);
    };
    return virtNetList;
}
QStringList NetControlThread::createNetwork()
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
    virNetworkPtr network = virNetworkCreateXML(currWorkConnect, xmlData.data());
    if ( network==NULL ) {
        sendConnErrors();
        return result;
    };
    result.append(QString("'%1' Network from\n\"%2\"\nis created.").arg(virNetworkGetName(network)).arg(path));
    virNetworkFree(network);
    return result;
}
QStringList NetControlThread::defineNetwork()
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
    virNetworkPtr network = virNetworkDefineXML(currWorkConnect, xmlData.data());
    if ( network==NULL ) {
        sendConnErrors();
        return result;
    };
    result.append(QString("'%1' Network from\n\"%2\"\nis defined.").arg(virNetworkGetName(network)).arg(path));
    virNetworkFree(network);
    return result;
}
QStringList NetControlThread::startNetwork()
{
    QStringList result;
    QString name = args.first();
    virNetworkPtr *network;
    unsigned int flags = VIR_CONNECT_LIST_NETWORKS_ACTIVE |
                         VIR_CONNECT_LIST_NETWORKS_INACTIVE;
    int ret = virConnectListAllNetworks( currWorkConnect, &network, flags);
    if ( ret<0 ) {
        sendConnErrors();
        free(network);
        return result;
    };
    //qDebug()<<QString(virConnectGetURI(currWorkConnect));

    int i = 0;
    bool started = false;
    while ( network[i] != NULL ) {
        QString currNetName = QString( virNetworkGetName(network[i]) );
        if ( !started && currNetName==name ) {
            started = (virNetworkCreate(network[i])+1) ? true : false;
            if (!started) sendGlobalErrors();
        };
        virNetworkFree(network[i]);
        i++;
    };
    free(network);
    result.append(QString("'%1' Network %2 Started.").arg(name).arg((started)?"":"don't"));
    return result;
}
QStringList NetControlThread::destroyNetwork()
{
    QStringList result;
    QString name = args.first();
    virNetworkPtr *network;
    unsigned int flags = VIR_CONNECT_LIST_NETWORKS_ACTIVE |
                         VIR_CONNECT_LIST_NETWORKS_INACTIVE;
    int ret = virConnectListAllNetworks( currWorkConnect, &network, flags);
    if ( ret<0 ) {
        sendConnErrors();
        free(network);
        return result;
    };
    //qDebug()<<QString(virConnectGetURI(currWorkConnect));

    int i = 0;
    bool deleted = false;
    while ( network[i] != NULL ) {
        QString currNetName = QString( virNetworkGetName(network[i]) );
        if ( !deleted && currNetName==name ) {
            deleted = (virNetworkDestroy(network[i])+1) ? true : false;
            if (!deleted) sendGlobalErrors();
        };
        virNetworkFree(network[i]);
        i++;
    };
    free(network);
    result.append(QString("'%1' Network %2 Destroyed.").arg(name).arg((deleted)?"":"don't"));
    return result;
}
QStringList NetControlThread::undefineNetwork()
{
    QStringList result;
    QString name = args.first();
    virNetworkPtr *network;
    unsigned int flags = VIR_CONNECT_LIST_NETWORKS_ACTIVE |
                         VIR_CONNECT_LIST_NETWORKS_INACTIVE;
    int ret = virConnectListAllNetworks( currWorkConnect, &network, flags);
    if ( ret<0 ) {
        sendConnErrors();
        free(network);
        return result;
    };
    //qDebug()<<QString(virConnectGetURI(currWorkConnect));

    int i = 0;
    bool deleted = false;
    while ( network[i] != NULL ) {
        QString currNetName = QString( virNetworkGetName(network[i]) );
        if ( !deleted && currNetName==name ) {
            deleted = (virNetworkUndefine(network[i])+1) ? true : false;
            if (!deleted) sendGlobalErrors();
        };
        virNetworkFree(network[i]);
        i++;
    };
    free(network);
    result.append(QString("'%1' Network %2 Undefined.").arg(name).arg((deleted)?"":"don't"));
    return result;
}
QStringList NetControlThread::changeAutoStartNetwork()
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
    virNetworkPtr *network;
    unsigned int flags = VIR_CONNECT_LIST_NETWORKS_ACTIVE |
                         VIR_CONNECT_LIST_NETWORKS_INACTIVE;
    int ret = virConnectListAllNetworks( currWorkConnect, &network, flags);
    if ( ret<0 ) {
        sendConnErrors();
        free(network);
        return result;
    };
    //qDebug()<<QString(virConnectGetURI(currWorkConnect));

    int i = 0;
    bool set = false;
    while ( network[i] != NULL ) {
        QString currNetName = QString( virNetworkGetName(network[i]) );
        if ( !set && currNetName==name ) {
            set = (virNetworkSetAutostart(network[i], autostart)+1) ? true : false;
            if (!set) sendGlobalErrors();
        };
        virNetworkFree(network[i]);
        i++;
    };
    free(network);
    result.append(QString("'%1' Network %2 Set.").arg(name).arg((set)?"":"don't"));
    return result;
}

void NetControlThread::sendConnErrors()
{
    virtErrors = virConnGetLastError(currWorkConnect);
    if ( virtErrors!=NULL ) {
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code).arg(virtErrors->message) );
        virResetError(virtErrors);
    };
}
void NetControlThread::sendGlobalErrors()
{
    virtErrors = virGetLastError();
    if ( virtErrors!=NULL )
        emit errorMsg( QString("VirtError(%1) : %2").arg(virtErrors->code).arg(virtErrors->message) );
    virResetLastError();
}
