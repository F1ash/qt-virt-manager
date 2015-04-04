#include "net_control_thread.h"

NetControlThread::NetControlThread(QObject *parent) :
    ControlThread(parent)
{
}

/* public slots */
void NetControlThread::execAction(Actions act, QStringList _args)
{
    if ( keep_alive && !isRunning() ) {
        action = act;
        args = _args;
        start();
    };
}

/* private slots */
void NetControlThread::run()
{
    Result result;
    switch (action) {
    case GET_ALL_ENTITY :
        result = getAllNetworkList();
        break;
    case CREATE_ENTITY :
        result = createNetwork();
        break;
    case DEFINE_ENTITY :
        result = defineNetwork();
        break;
    case START_ENTITY :
        result = startNetwork();
        break;
    case DESTROY_ENTITY :
        result = destroyNetwork();
        break;
    case UNDEFINE_ENTITY :
        result = undefineNetwork();
        break;
    case CHANGE_ENTITY_AUTOSTART :
        result = changeAutoStartNetwork();
        break;
    case GET_XML_DESCRIPTION :
        result = getVirtNetXMLDesc();
        break;
    default:
        break;
    };
    result.type   = "network";
    result.number = number;
    result.action = action;
    emit resultData(result);
}
Result NetControlThread::getAllNetworkList()
{
    Result result;
    QStringList virtNetList;
    if ( currWorkConnect!=NULL && keep_alive ) {
        virNetworkPtr *network;
        unsigned int flags = VIR_CONNECT_LIST_NETWORKS_ACTIVE |
                             VIR_CONNECT_LIST_NETWORKS_INACTIVE;
        int ret = virConnectListAllNetworks( currWorkConnect, &network, flags);
        if ( ret<0 ) {
            sendConnErrors();
            free(network);
            return result;
        };

        int i = 0;
        while ( network[i] != NULL ) {
            QStringList currentAttr;
            QString autostartStr;
            int is_autostart = 0;
            if (virNetworkGetAutostart(network[i], &is_autostart) < 0) {
                autostartStr.append("no autostart");
            } else autostartStr.append( is_autostart ? "yes" : "no" );
            currentAttr<< QString().fromUtf8( virNetworkGetName(network[i]) )
                       << QString( virNetworkIsActive(network[i]) ? "active" : "inactive" )
                       << autostartStr
                       << QString( virNetworkIsPersistent(network[i]) ? "yes" : "no" );
            virtNetList.append(currentAttr.join(DFR));
            //qDebug()<<currentAttr;
            virNetworkFree(network[i]);
            i++;
        };
        free(network);
    };
    result.result = true;
    result.msg = virtNetList;
    return result;
}
Result NetControlThread::createNetwork()
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
    virNetworkPtr network = virNetworkCreateXML(currWorkConnect, xmlData.data());
    if ( network==NULL ) {
        sendConnErrors();
        return result;
    };
    result.name = QString().fromUtf8( virNetworkGetName(network) );
    result.result = true;
    result.msg.append(QString("'<b>%1</b>' Network from\n\"%2\"\nis created.")
                      .arg(result.name).arg(path));
    virNetworkFree(network);
    return result;
}
Result NetControlThread::defineNetwork()
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
    virNetworkPtr network = virNetworkDefineXML(currWorkConnect, xmlData.data());
    if ( network==NULL ) {
        sendConnErrors();
        return result;
    };
    result.name = QString().fromUtf8( virNetworkGetName(network) );
    result.result = true;
    result.msg.append(QString("'<b>%1</b>' Network from\n\"%2\"\nis defined.")
                      .arg(result.name).arg(path));
    virNetworkFree(network);
    return result;
}
Result NetControlThread::startNetwork()
{
    Result result;
    QString name = args.first();
    bool started = false;
    virNetworkPtr network = virNetworkLookupByName(currWorkConnect, name.toUtf8().data());
    if ( network!=NULL ) {
        started = (virNetworkCreate(network)+1) ? true : false;
        if (!started) sendConnErrors();
        virNetworkFree(network);
    } else sendConnErrors();
    result.name = name;
    result.result = started;
    result.msg.append(QString("'<b>%1</b>' Network %2 Started.")
                      .arg(name).arg((started)?"":"don't"));
    return result;
}
Result NetControlThread::destroyNetwork()
{
    Result result;
    QString name = args.first();
    bool deleted = false;
    virNetworkPtr network = virNetworkLookupByName(currWorkConnect, name.toUtf8().data());
    if ( network!=NULL ) {
        deleted = (virNetworkDestroy(network)+1) ? true : false;
        if (!deleted) sendConnErrors();
        virNetworkFree(network);
    } else sendConnErrors();
    result.name = name;
    result.result = deleted;
    result.msg.append(QString("'<b>%1</b>' Network %2 Destroyed.")
                      .arg(name).arg((deleted)?"":"don't"));
    return result;
}
Result NetControlThread::undefineNetwork()
{
    Result result;
    QString name = args.first();
    bool deleted = false;
    virNetworkPtr network = virNetworkLookupByName(currWorkConnect, name.toUtf8().data());
    if ( network!=NULL ) {
        deleted = (virNetworkUndefine(network)+1) ? true : false;
        if (!deleted) sendConnErrors();
        virNetworkFree(network);
    } else sendConnErrors();
    result.name = name;
    result.result = deleted;
    result.msg.append(QString("'<b>%1</b>' Network %2 Undefined.")
                      .arg(name).arg((deleted)?"":"don't"));
    return result;
}
Result NetControlThread::changeAutoStartNetwork()
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
    virNetworkPtr network = virNetworkLookupByName(currWorkConnect, name.toUtf8().data());
    if ( network!=NULL ) {
        set = (virNetworkSetAutostart(network, autostart)+1) ? true : false;
        if (!set) sendConnErrors();
        virNetworkFree(network);
    } else sendConnErrors();
    result.result = set;
    result.msg.append(QString("'<b>%1</b>' Network autostart %2 Set.")
                      .arg(name).arg((set)?"":"don't"));
    return result;
}
Result NetControlThread::getVirtNetXMLDesc()
{
    Result result;
    QString name = args.first();
    result.name = name;
    bool read = false;
    char *Returns = NULL;
    virNetworkPtr network = virNetworkLookupByName(currWorkConnect, name.toUtf8().data());
    if ( network!=NULL ) {
        Returns = (virNetworkGetXMLDesc(network, VIR_NETWORK_XML_INACTIVE));
        if ( Returns==NULL ) sendConnErrors();
        else read = true;
        virNetworkFree(network);
    } else sendConnErrors();
    QTemporaryFile f;
    f.setAutoRemove(false);
    f.setFileTemplate(QString("%1%2XML_Desc-XXXXXX.xml").arg(QDir::tempPath()).arg(QDir::separator()));
    read = f.open();
    if (read) f.write(Returns);
    result.msg.append(f.fileName());
    f.close();
    free(Returns);
    result.result = read;
    result.msg.append(QString("'<b>%1</b>' Network %2 XML'ed")
                      .arg(name).arg((read)?"":"don't"));
    return result;
}
