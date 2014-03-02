#include "net_control_thread.h"

NetControlThread::NetControlThread(QObject *parent) :
    QThread(parent)
{
}

/* public slots */
bool NetControlThread::setCurrentWorkConnect(virConnectPtr conn)
{
    keep_alive = true;
    currWorkConnect = conn;
    qDebug()<<"net_thread"<<currWorkConnect;
}
void NetControlThread::stop() { keep_alive = false; }
void NetControlThread::execAction(Actions act = GET_ALL_NETWORK,
                                  QStringList _args = QStringList())
{
    action = act;
    args = _args;
    if ( keep_alive && !isRunning() ) start();
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
        result.append(QString());
        break;
    };
    result.prepend(QString::number(action));

    emit resultData(result);
    //qDebug()<<"netControlThread stopped";
}
QStringList NetControlThread::getAllNetworkList()
{
    QStringList virtNetList;
    if ( currWorkConnect!=NULL && keep_alive ) {
        virNetworkPtr *network;
        virErrorPtr error;
        unsigned int flags = VIR_CONNECT_LIST_NETWORKS_ACTIVE |
                             VIR_CONNECT_LIST_NETWORKS_INACTIVE;
        int ret = virConnectListAllNetworks( currWorkConnect, &network, flags);
        if ( ret<0 ) {
            error = virConnGetLastError(currWorkConnect);
            free(network);
            QString err = QString("%1 : %2").arg(error->code).arg(error->message);
            emit errorMsg(err);
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
    return result;
}
QStringList NetControlThread::defineNetwork()
{
    QStringList result;
    return result;
}
QStringList NetControlThread::startNetwork()
{
    QStringList result;
    return result;
}
QStringList NetControlThread::destroyNetwork()
{
    QStringList result;
    return result;
}
QStringList NetControlThread::undefineNetwork()
{
    QStringList result;
    return result;
}
QStringList NetControlThread::changeAutoStartNetwork()
{
    QStringList result;
    return result;
}
