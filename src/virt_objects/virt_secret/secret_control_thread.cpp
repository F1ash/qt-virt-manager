#include "secret_control_thread.h"
#include <QDomDocument>

SecretControlThread::SecretControlThread(QObject *parent) :
    ControlThread(parent)
{
}

/* public slots */
void SecretControlThread::execAction(uint _num, TASK _task)
{
    number = _num;
    task = _task;
    if ( NULL!=task.srcConnPtr ) {
        // for new virConnect usage create the new virConnectRef[erence]
        int ret = virConnectRef(*task.srcConnPtr);
        if ( ret<0 ) {
            task.srcConnPtr = NULL;
            sendConnErrors();
            keep_alive = false;
        } else
            keep_alive = true;
    };
    if ( keep_alive && !isRunning() ) {
        currConnPtr = task.srcConnPtr;
        start();
    } else {
        Result result;
        result.type   = "secret";
        result.number = number;
        result.action = _EMPTY_ACTION;
        emit resultData(result);
    };
}

/* private slots */
void SecretControlThread::run()
{
    Result result;
    switch (task.action) {
    case GET_ALL_ENTITY_STATE :
        result = getAllSecretList();
        break;
    case DEFINE_ENTITY :
        result = defineSecret();
        break;
    case UNDEFINE_ENTITY :
        result = undefineSecret();
        break;
    case GET_XML_DESCRIPTION :
        result = getVirtSecretXMLDesc();
        break;
    default:
        break;
    };
    // task.srcConnPtr reference will closed in destructor as currConnPtr
    //virConnectClose(*task.srcConnPtr);
    result.type   = "secret";
    result.number = number;
    result.action = task.action;
    emit resultData(result);
}
Result SecretControlThread::getAllSecretList()
{
    Result result;
    QStringList virtSecretList;
    if ( task.srcConnPtr!=NULL && keep_alive ) {
        virSecretPtr *secrets = NULL;
        //extra flags; not used yet, so callers should always pass 0
        unsigned int flags = 0;
        int ret = virConnectListAllSecrets(*task.srcConnPtr, &secrets, flags);
        if ( ret<0 ) {
            result.err = sendConnErrors();
            return result;
        };

        // therefore correctly to use for() command, because secrets[0] can not exist.
        for (int i = 0; i < ret; i++) {
            QStringList currentAttr;
            char uuid[100];
            if ( 0>virSecretGetUUIDString(secrets[i], uuid) )
                uuid[0] = '?';
            const char* usageID = virSecretGetUsageID(secrets[i]);

            QString type;
            switch (virSecretGetUsageType(secrets[i])) {
            case VIR_SECRET_USAGE_TYPE_NONE:
                type.append("NONE");
                break;
            case VIR_SECRET_USAGE_TYPE_VOLUME:
                type.append("VOLUME");
                break;
            case VIR_SECRET_USAGE_TYPE_CEPH:
                type.append("CEPH");
                break;
            case VIR_SECRET_USAGE_TYPE_ISCSI:
                type.append("ISCSI");
                break;
            default:
                type.append("NONE");
                break;
            };
            char *xmlDesc = NULL;
            xmlDesc = virSecretGetXMLDesc(secrets[i], 0);
            QDomDocument doc;
            doc.setContent(QString::fromUtf8(xmlDesc));
            QString desc = doc.firstChildElement("secret")
                    .firstChildElement("description").text();
            //size_t value_size;
            //unsigned char *value = virSecretGetValue(secrets[i], &value_size, 0);
            currentAttr<< uuid\
                       <<QString::fromUtf8(usageID)\
                       <<type\
                       <<desc;
            virtSecretList.append(currentAttr.join(DFR));
            //qDebug() << currentAttr << value_size;
            //for (int k=0; k< value_size; k++) {
            //    if (value[k]=='\0') break;
            //    qDebug()<<( value[k] );
            //};
            virSecretFree(secrets[i]);
        };
        free(secrets);
    };
    result.result = true;
    result.msg = virtSecretList;
    return result;
}
Result SecretControlThread::defineSecret()
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
    //extra flags; not used yet, so callers should always pass 0
    int flags = 0;
    virSecretPtr secret = virSecretDefineXML(
                *task.srcConnPtr, xmlData.data(), flags);
    if ( secret==NULL ) {
        result.err = sendConnErrors();
        return result;
    };
    unsigned char *value = (unsigned char*)(
                task.secret->getSecretValue().data());
    size_t value_size = task.secret->getSecretValue().length();
    //qDebug()<<task.secret->getSecretValue().data()<<(const char*)(value)<<value_size;
    //extra flags; not used yet, so callers should always pass 0
    flags = 0;
    int ret = virSecretSetValue(secret, value, value_size, flags);
    char uuid[100];
    virSecretGetUUIDString(secret, uuid);
    result.name = QString::fromUtf8(uuid);
    result.result = true;
    result.msg.append(
                QString("'<b>%1</b>' Secret from\n\"%2\"\nis defined.")
                .arg(result.name).arg(path));
    if ( ret<0 ) result.msg.append("Secret Value not set.");
    virSecretFree(secret);
    return result;
}
Result SecretControlThread::undefineSecret()
{
    Result result;
    QString uuid = task.object;
    bool deleted = false;
    virSecretPtr secret = virSecretLookupByUUIDString(
                *task.srcConnPtr, uuid.toUtf8().data());
    if ( secret!=NULL ) {
        deleted = (virSecretUndefine(secret)+1) ? true : false;
        if (!deleted)
            result.err = sendConnErrors();
        virSecretFree(secret);
    } else
        result.err = sendConnErrors();
    result.name = uuid;
    result.result = deleted;
    result.msg.append(QString("'<b>%1</b>' Secret %2 Undefined.")
                      .arg(uuid).arg((deleted)?"":"don't"));
    return result;
}
Result SecretControlThread::getVirtSecretXMLDesc()
{
    Result result;
    QString uuid = task.object;
    result.name = uuid;
    bool read = false;
    char *Returns = NULL;
    virSecretPtr secret = virSecretLookupByUUIDString(
                *task.srcConnPtr, uuid.toUtf8().data());
    if ( secret!=NULL ) {
        //extra flags; not used yet, so callers should always pass 0
        int flags = 0;
        Returns = (virSecretGetXMLDesc(secret, flags));
        if ( Returns==NULL )
            result.err = sendConnErrors();
        else read = true;
        virSecretFree(secret);
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
    result.result = read;
    result.msg.append(QString("'<b>%1</b>' Secret %2 XML'ed")
                      .arg(uuid).arg((read)?"":"don't"));
    return result;
}
