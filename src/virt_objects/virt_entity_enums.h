#ifndef VIRT_ENTITY_ENUMS_H
#define VIRT_ENTITY_ENUMS_H

#include <QString>
#include <QObject>

enum class VIRT_ENTITY {
    _NONE_ENTITY,
    VIRT_DOMAIN,
    VIRT_NETWORK,
    VIRT_STORAGE_POOL,
    VIRT_STORAGE_VOLUME,
    VIRT_SECRET,
    VIRT_INTERFACE,
    VIRT_NETWORK_FILTER
};

enum class Actions {
    _NONE_ACTION,
    GET_ALL_ENTITY_STATE,
    GET_ALL_ENTITY_DATA0,
    GET_ALL_ENTITY_DATA1,
    CREATE_ENTITY,
    DEFINE_ENTITY,
    EDIT_ENTITY,
    START_ENTITY,
    PAUSE_ENTITY,
    DESTROY_ENTITY,
    RESET_ENTITY,
    REBOOT_ENTITY,
    SHUTDOWN_ENTITY,
    SAVE_ENTITY,
    RESTORE_ENTITY,
    UNDEFINE_ENTITY,
    DELETE_ENTITY,
    MIGRATE_ENTITY,
    DOWNLOAD_ENTITY,
    RESIZE_ENTITY,
    UPLOAD_ENTITY,
    WIPE_ENTITY,
    REFRESH_ENTITY,
    CHANGE_ENTITY_AUTOSTART,
    GET_XML_DESCRIPTION,
    CREATE_DOMAIN_SNAPSHOT,
    REVERT_TO_DOMAIN_SNAPSHOT,
    DELETE_DOMAIN_SNAPSHOT,
    IFACE_CHANGE_BEGIN,
    IFACE_CHANGE_COMMIT,
    IFACE_CHANGE_ROLLBACK
};

enum class Methods {
    _NONE_METHOD,
    createEntity,
    defineEntity,
    undefineEntity,
    startEntity,
    pauseEntity,
    destroyEntity,
    resetVirtDomain,
    rebootVirtDomain,
    shutdownVirtDomain,
    saveVirtDomain,
    restoreVirtDomain,
    setAutostartEntity,
    migrateVirtDomain,
    monitorVirtDomain,
    getEntityXMLDesc,
    displayVirtDomain,
    displayVirtDomainInExternalViewer,
    createVirtDomainSnapshot,
    moreSnapshotActions,
    revertVirtDomainSnapshot,
    deleteVirtDomainSnapshot,
    reconnectToVirtDomain,
    editEntity,
    reloadEntity,
    deleteEntity,
    overviewEntity,
    downloadVirtStorageVol,
    resizeVirtStorageVol,
    uploadVirtStorageVol,
    wipeVirtStorageVol,
    refreshVirtStorageVolList,
    changeBeginVirtInterface,
    changeCommitVirtInterface,
    changeRollbackVirtInterface,
    sendKeySeqToVirtDomain,
    getScreenshotFromVirtDomain,
    copyFilesToVirtDomain,
    cancelCopyFilesToVirtDomain,
    copyToClipboardFromVirtDomain,
    pasteClipboardToVirtDomain,
    fullScreenVirtDomain,
    scaleScreenVirtDomain
};

class _TO_STRING : public QObject
{
public:

    static QString enumToEntityString(VIRT_ENTITY entity)
    {
        switch (entity) {
        case VIRT_ENTITY::VIRT_DOMAIN:
            return tr("domain");
        case VIRT_ENTITY::VIRT_NETWORK:
            return tr("network");
        case VIRT_ENTITY::VIRT_STORAGE_POOL:
            return tr("pool");
        case VIRT_ENTITY::VIRT_STORAGE_VOLUME:
            return tr("volume");
        case VIRT_ENTITY::VIRT_INTERFACE:
            return tr("interface");
        case VIRT_ENTITY::VIRT_SECRET:
            return tr("secret");
        case VIRT_ENTITY::VIRT_NETWORK_FILTER:
            return tr("network filter");
        case VIRT_ENTITY::_NONE_ENTITY:
        default:
            break;
        };
        return "";
    }

    static QString enumToActionString(Actions act)
    {
        switch (act) {
        case Actions::CREATE_ENTITY:
            return tr("create");
        case Actions::DEFINE_ENTITY:
            return tr("define");
        case Actions::UNDEFINE_ENTITY:
            return tr("undefine");
        case Actions::DESTROY_ENTITY:
            return tr("destroy");
        case Actions::EDIT_ENTITY:
            return tr("edit");
        case Actions::MIGRATE_ENTITY:
            return tr("migrate");
        case Actions::_NONE_ACTION:
        default:
            break;
        };
        return "";
    }

    static QString enumToMethodString(Methods method)
    {
        switch (method) {
        case Methods::createEntity:
            return tr("create");
        case Methods::defineEntity:
            return tr("define");
        case Methods::undefineEntity:
            return tr("undefine");
        case Methods::startEntity:
            return tr("start");
        case Methods::pauseEntity:
            return tr("pause");
        case Methods::destroyEntity:
            return tr("destroy");
        case Methods::resetVirtDomain:
            return tr("reset");
        case Methods::rebootVirtDomain:
            return tr("reboot");
        case Methods::shutdownVirtDomain:
            return tr("shutdown");
        case Methods::saveVirtDomain:
            return tr("save");
        case Methods::restoreVirtDomain:
            return tr("restore");
        case Methods::setAutostartEntity:
            return tr("set autostart");
        case Methods::migrateVirtDomain:
            return tr("migrate");
        case Methods::monitorVirtDomain:
            return tr("to monitor");
        case Methods::getEntityXMLDesc:
            return tr("get XML");
        case Methods::displayVirtDomain:
        case Methods::displayVirtDomainInExternalViewer:
            return tr("display");
        case Methods::createVirtDomainSnapshot:
            return tr("create snapshot");
        //case Methods::moreSnapshotActions:
        //    return tr("display");
        case Methods::revertVirtDomainSnapshot:
            return tr("revert to snapshot");
        case Methods::deleteVirtDomainSnapshot:
            return tr("delete snapshot");
        case Methods::reconnectToVirtDomain:
            return tr("reconnect");
        case Methods::editEntity:
            return tr("edit");
        case Methods::reloadEntity:
            return tr("reload");
        case Methods::deleteEntity:
            return tr("delete");
        case Methods::overviewEntity:
            return tr("overview");
        case Methods::downloadVirtStorageVol:
            return tr("download");
        case Methods::resizeVirtStorageVol:
            return tr("resize");
        case Methods::uploadVirtStorageVol:
            return tr("upload");
        case Methods::wipeVirtStorageVol:
            return tr("wipe");
        case Methods::refreshVirtStorageVolList:
            return tr("refresh Pool");
        case Methods::changeBeginVirtInterface:
        case Methods::changeCommitVirtInterface:
        case Methods::changeRollbackVirtInterface:
        case Methods::sendKeySeqToVirtDomain:
        case Methods::getScreenshotFromVirtDomain:
        case Methods::copyFilesToVirtDomain:
        case Methods::cancelCopyFilesToVirtDomain:
        case Methods::copyToClipboardFromVirtDomain:
        case Methods::pasteClipboardToVirtDomain:
        case Methods::fullScreenVirtDomain:
        case Methods::scaleScreenVirtDomain:
        case Methods::_NONE_METHOD:
        default:
            break;
        };
        return "";
    }

};

enum class HOW_TO_DO {
    _NONE_DO,
    DO_AsIs,
    DO_Edit,
    DO_Manually
};

struct Act_Param {
    Actions     act;
    Methods     method;
    HOW_TO_DO   context;
    QString     path;
    void clear() {
        act     = Actions::_NONE_ACTION;
        context = HOW_TO_DO::_NONE_DO;
        path.clear();
    }
};

#endif // VIRT_ENTITY_ENUMS_H
