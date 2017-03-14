#include "nwfilter_control_thread.h"
#include <QDomDocument>

NWFilterControlThread::NWFilterControlThread(QObject *parent) :
    ControlThread(parent)
{
}

/* public slots */
void NWFilterControlThread::execAction(uint _num, TASK _task)
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
        result.type   = VIRT_ENTITY::VIRT_NETWORK_FILTER;
        result.number = number;
        result.action = Actions::_NONE_ACTION;
        emit resultData(result);
    };
}

/* private slots */
void NWFilterControlThread::run()
{
    Result result;
    switch (task.action) {
    case Actions::GET_ALL_ENTITY_STATE :
        result = getAllNWFilterList();
        break;
    case Actions::DEFINE_ENTITY :
        result = defineNWFilter();
        break;
    case Actions::UNDEFINE_ENTITY :
        result = undefineNWFilter();
        break;
    case Actions::EDIT_ENTITY:
    case Actions::GET_XML_DESCRIPTION :
        result = getVirtNWFilterXMLDesc();
        break;
    default:
        break;
    };
    // task.srcConnPtr reference will closed in destructor as ptr_ConnPtr
    //virConnectClose(*task.srcConnPtr);
    result.type   = VIRT_ENTITY::VIRT_NETWORK_FILTER;
    result.number = number;
    result.action = task.action;
    emit resultData(result);
}
Result NWFilterControlThread::getAllNWFilterList()
{
    Result result;
    ACT_RESULT virtNWFilterList;
    if ( task.srcConnPtr!=nullptr && keep_alive ) {
        virNWFilterPtr *filters = nullptr;
        //extra flags; not used yet, so callers should always pass 0
        unsigned int flags = 0;
        int ret = virConnectListAllNWFilters(
                    *task.srcConnPtr, &filters, flags);
        if ( ret<0 ) {
            result.err = sendConnErrors();
            return result;
        };

        // therefore correctly to use for() command,
        // because filters[0] can not exist.
        for (int i = 0; i < ret; i++) {
            QVariantMap currentAttr;
            char uuid[100];
            if ( 0>virNWFilterGetUUIDString(filters[i], uuid) )
                uuid[0] = '?';
            const char* name = virNWFilterGetName(filters[i]);

            currentAttr.insert(
                        "name", QString::fromUtf8(name));
            currentAttr.insert(
                        "UUID", uuid);
            virtNWFilterList.append(currentAttr);
            virNWFilterFree(filters[i]);
        };
        if (filters) free(filters);
    };
    result.result = true;
    result.data   = virtNWFilterList;
    return result;
}
Result NWFilterControlThread::defineNWFilter()
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
    virNWFilterPtr filter = virNWFilterDefineXML(
                *task.srcConnPtr, xmlData.data());
    if ( filter==nullptr ) {
        result.err = sendConnErrors();
        return result;
    };
    const char *name = virNWFilterGetName(filter);
    result.name = QString::fromUtf8(name);
    result.result = true;
    result.msg.append(
                QString("'<b>%1</b>' NWFilter from\n\"%2\"\nis defined.")
                .arg(result.name).arg(path));
    virNWFilterFree(filter);
    return result;
}
Result NWFilterControlThread::undefineNWFilter()
{
    Result result;
    QString name = task.object;
    bool deleted = false;
    if ( task.srcConnPtr==nullptr ) {
        result.result = false;
        result.err = "Connection pointer is NULL.";
        return result;
    };
    virNWFilterPtr filter = virNWFilterLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( filter!=nullptr ) {
        deleted = (virNWFilterUndefine(filter)+1) ? true : false;
        if (!deleted)
            result.err = sendConnErrors();
        virNWFilterFree(filter);
    } else
        result.err = sendConnErrors();
    result.name = name;
    result.result = deleted;
    result.msg.append(QString("'<b>%1</b>' NWFilter %2 Undefined.")
                      .arg(name).arg((deleted)?"":"don't"));
    return result;
}
Result NWFilterControlThread::getVirtNWFilterXMLDesc()
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
    virNWFilterPtr filter = virNWFilterLookupByName(
                *task.srcConnPtr, name.toUtf8().data());
    if ( filter!=nullptr ) {
        //extra flags; not used yet, so callers should always pass 0
        int flags = 0;
        Returns = (virNWFilterGetXMLDesc(filter, flags));
        if ( Returns==nullptr )
            result.err = sendConnErrors();
        else read = true;
        virNWFilterFree(filter);
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
    result.msg.append(QString("'<b>%1</b>' NWFilter %2 XML'ed")
                      .arg(name).arg((read)?"":"don't"));
    return result;
}
