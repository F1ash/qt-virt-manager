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
        result.type   = VIRT_ENTITY::VIRT_SECRET;
        result.number = number;
        result.action = Actions::_NONE_ACTION;
        emit resultData(result);
    };
}

/* private slots */
void SecretControlThread::run()
{
    Result result;
    switch (task.action) {
    case Actions::GET_ALL_ENTITY_STATE :
        result = getAllSecretList();
        break;
    case Actions::DEFINE_ENTITY :
        result = defineSecret();
        break;
    case Actions::UNDEFINE_ENTITY :
        result = undefineSecret();
        break;
    case Actions::GET_XML_DESCRIPTION :
        result = getVirtSecretXMLDesc();
        break;
    default:
        break;
    };
    // task.srcConnPtr reference will closed in destructor as ptr_ConnPtr
    //virConnectClose(*task.srcConnPtr);
    result.type   = VIRT_ENTITY::VIRT_SECRET;
    result.number = number;
    result.action = task.action;
    emit resultData(result);
}
Result SecretControlThread::getAllSecretList()
{
    Result result;
    ACT_RESULT virtSecretList;
    if ( task.srcConnPtr!=nullptr && keep_alive ) {
        virSecretPtr *secrets = nullptr;
        //extra flags; not used yet, so callers should always pass 0
        unsigned int flags = 0;
        int ret = virConnectListAllSecrets(
                    *task.srcConnPtr, &secrets, flags);
        if ( ret<0 ) {
            result.err = sendConnErrors();
            return result;
        };

        // therefore correctly to use for() command,
        // because secrets[0] can not exist.
        for (int i = 0; i < ret; i++) {
            QVariantMap currentAttr;
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
            case VIR_SECRET_USAGE_TYPE_TLS:
                type.append("TLS");
                break;
            default:
                type.append("NONE");
                break;
            };
            char *xmlDesc = nullptr;
            xmlDesc = virSecretGetXMLDesc(secrets[i], 0);
            QDomDocument doc;
            doc.setContent(QString::fromUtf8(xmlDesc));
            QString desc = doc.firstChildElement("secret")
                    .firstChildElement("description").text();
            //size_t value_size;
            //unsigned char *value = virSecretGetValue(secrets[i], &value_size, 0);
            currentAttr.insert(
                        "UUID", QString::fromUtf8(uuid));
            currentAttr.insert(
                        "UsageID", QString::fromUtf8(usageID));
            currentAttr.insert(
                        "desc", desc);
            currentAttr.insert(
                        "type", type);
            virtSecretList.append(currentAttr);
            //qDebug() << currentAttr << value_size;
            //for (int k=0; k< value_size; k++) {
            //    if (value[k]=='\0') break;
            //    qDebug()<<( value[k] );
            //};
            virSecretFree(secrets[i]);
            if (xmlDesc) free(xmlDesc);
        };
        if (secrets) free(secrets);
    };
    result.result = true;
    result.data   = virtSecretList;
    return result;
}
Result SecretControlThread::defineSecret()
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
    //extra flags; not used yet, so callers should always pass 0
    int flags = 0;
    virSecretPtr secret = virSecretDefineXML(
                *task.srcConnPtr, xmlData.data(), flags);
    if ( secret==nullptr ) {
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
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
    virSecretPtr secret = virSecretLookupByUUIDString(
                *task.srcConnPtr, uuid.toUtf8().data());
    if ( secret!=nullptr ) {
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
    char *Returns = nullptr;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
    virSecretPtr secret = virSecretLookupByUUIDString(
                *task.srcConnPtr, uuid.toUtf8().data());
    if ( secret!=nullptr ) {
        //extra flags; not used yet, so callers should always pass 0
        int flags = 0;
        Returns = (virSecretGetXMLDesc(secret, flags));
        if ( Returns==nullptr )
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
    if ( Returns!=nullptr ) free(Returns);
    result.result = read;
    result.msg.append(QString("'<b>%1</b>' Secret %2 XML'ed")
                      .arg(uuid).arg((read)?"":"don't"));
    return result;
}
