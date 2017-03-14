#ifndef VIRT_ENTITY_ENUMS_H
#define VIRT_ENTITY_ENUMS_H

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

static const char* enumToEntityString(VIRT_ENTITY entity)
{
    switch (entity) {
    case VIRT_ENTITY::VIRT_DOMAIN:
        return "domain";
    case VIRT_ENTITY::VIRT_NETWORK:
        return "network";
    case VIRT_ENTITY::VIRT_STORAGE_POOL:
        return "pool";
    case VIRT_ENTITY::VIRT_STORAGE_VOLUME:
        return "volume";
    case VIRT_ENTITY::VIRT_INTERFACE:
        return "interface";
    case VIRT_ENTITY::VIRT_SECRET:
        return "secret";
    case VIRT_ENTITY::VIRT_NETWORK_FILTER:
        return "network filter";
    case VIRT_ENTITY::_NONE_ENTITY:
    default:
        break;
    };
    return "";
}

static const char* enumToActionString(Actions act)
{
    switch (act) {
    case Actions::CREATE_ENTITY:
        return "create";
    case Actions::DEFINE_ENTITY:
        return "define";
    case Actions::UNDEFINE_ENTITY:
        return "undefine";
    case Actions::DESTROY_ENTITY:
        return "destroy";
    case Actions::EDIT_ENTITY:
        return "edit";
    case Actions::MIGRATE_ENTITY:
        return "migrate";
    case Actions::_NONE_ACTION:
    default:
        break;
    };
    return "";
}

static const char* enumToMethodString(Methods method)
{
    switch (method) {
    case Methods::createEntity:
        return "create";
    case Methods::defineEntity:
        return "define";
    case Methods::undefineEntity:
        return "undefine";
    case Methods::startEntity:
        return "start";
    case Methods::pauseEntity:
        return "pause";
    case Methods::destroyEntity:
        return "destroy";
    case Methods::resetVirtDomain:
        return "reset";
    case Methods::rebootVirtDomain:
        return "reboot";
    case Methods::shutdownVirtDomain:
        return "shutdown";
    case Methods::saveVirtDomain:
        return "save";
    case Methods::restoreVirtDomain:
        return "restore";
    case Methods::setAutostartEntity:
        return "set autostart";
    case Methods::migrateVirtDomain:
        return "migrate";
    case Methods::monitorVirtDomain:
        return "to monitor";
    case Methods::getEntityXMLDesc:
        return "get XML";
    case Methods::displayVirtDomain:
    case Methods::displayVirtDomainInExternalViewer:
        return "display";
    case Methods::createVirtDomainSnapshot:
        return "create snapshot";
    //case Methods::moreSnapshotActions:
    //    return "display";
    case Methods::revertVirtDomainSnapshot:
        return "revert to snapshot";
    case Methods::deleteVirtDomainSnapshot:
        return "delete snapshot";
    case Methods::reconnectToVirtDomain:
        return "reconnect";
    case Methods::editEntity:
        return "edit";
    case Methods::reloadEntity:
        return "reload";
    case Methods::deleteEntity:
        return "delete";
    case Methods::overviewEntity:
        return "overview";
    case Methods::downloadVirtStorageVol:
        return "download";
    case Methods::resizeVirtStorageVol:
        return "resize";
    case Methods::uploadVirtStorageVol:
        return "upload";
    case Methods::wipeVirtStorageVol:
        return "wipe";
    case Methods::refreshVirtStorageVolList:
        return "refresh Pool";
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

enum class HOW_TO_DO {
    _NONE_DO,
    DO_AsIs,
    DO_Edit,
    DO_Manually
};

#include <QString>
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
