#ifndef VIRT_ENTITY_ENUMS_H
#define VIRT_ENTITY_ENUMS_H

enum VIRT_ENTITY {
    _NONE_ENTITY,
    VIRT_DOMAIN,
    VIRT_NETWORK,
    VIRT_STORAGE_POOL,
    VIRT_STORAGE_VOLUME,
    VIRT_SECRET,
    VIRT_INTERFACE,
    VIRT_NETWORK_FILTER
};

enum Actions {
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

enum Methods {
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
    reconnectToVirtDomainMethod,
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
    sendKeySeqToVirtDomainMethod,
    getScreenshotFromVirtDomainMethod,
    copyFilesToVirtDomainMethod,
    cancelCopyFilesToVirtDomainMethod,
    copyToClipboardFromVirtDomainMethod,
    pasteClipboardToVirtDomainMethod,
    fullScreenVirtDomainMethod,
    scaledScreenVirtDomainMethod
};

static const char* enumToEntityString(VIRT_ENTITY entity)
{
    switch (entity) {
    case VIRT_DOMAIN:
        return "domain";
    case VIRT_NETWORK:
        return "network";
    case VIRT_STORAGE_POOL:
        return "pool";
    case VIRT_STORAGE_VOLUME:
        return "volume";
    case VIRT_INTERFACE:
        return "interface";
    case VIRT_SECRET:
        return "secret";
    case VIRT_NETWORK_FILTER:
        return "network filter";
    case _NONE_ENTITY:
    default:
        break;
    };
    return "";
}

static const char* enumToActionString(Actions act)
{
    switch (act) {
    case CREATE_ENTITY:
        return "create";
    case DEFINE_ENTITY:
        return "define";
    case UNDEFINE_ENTITY:
        return "undefine";
    case DESTROY_ENTITY:
        return "destroy";
    case EDIT_ENTITY:
        return "edit";
    case MIGRATE_ENTITY:
        return "migrate";
    case _NONE_ACTION:
    default:
        break;
    };
    return "";
}

static const char* enumToMethodString(Methods method)
{
    switch (method) {
    case createEntity:
        return "create";
    case defineEntity:
        return "define";
    case undefineEntity:
        return "undefine";
    case startEntity:
        return "start";
    case pauseEntity:
        return "pause";
    case destroyEntity:
        return "destroy";
    case resetVirtDomain:
        return "reset";
    case rebootVirtDomain:
        return "reboot";
    case shutdownVirtDomain:
        return "shutdown";
    case saveVirtDomain:
        return "save";
    case restoreVirtDomain:
        return "restore";
    case setAutostartEntity:
        return "set autostart";
    case migrateVirtDomain:
        return "migrate";
    case monitorVirtDomain:
        return "to monitor";
    case getEntityXMLDesc:
        return "get XML";
    case displayVirtDomain:
    case displayVirtDomainInExternalViewer:
        return "display";
    case createVirtDomainSnapshot:
        return "create snapshot";
    //case moreSnapshotActions:
    //    return "display";
    case revertVirtDomainSnapshot:
        return "revert to snapshot";
    case deleteVirtDomainSnapshot:
        return "delete snapshot";
    case reconnectToVirtDomainMethod:
        return "reconnect";
    case editEntity:
        return "edit";
    case reloadEntity:
        return "reload";
    case deleteEntity:
        return "delete";
    case overviewEntity:
        return "overview";
    case downloadVirtStorageVol:
        return "download";
    case resizeVirtStorageVol:
        return "resize";
    case uploadVirtStorageVol:
        return "upload";
    case wipeVirtStorageVol:
        return "wipe";
    case refreshVirtStorageVolList:
        return "refresh Pool";
    case changeBeginVirtInterface:
    case changeCommitVirtInterface:
    case changeRollbackVirtInterface:
    case sendKeySeqToVirtDomainMethod:
    case getScreenshotFromVirtDomainMethod:
    case copyFilesToVirtDomainMethod:
    case cancelCopyFilesToVirtDomainMethod:
    case copyToClipboardFromVirtDomainMethod:
    case pasteClipboardToVirtDomainMethod:
    case fullScreenVirtDomainMethod:
    case scaledScreenVirtDomainMethod:
    case _NONE_METHOD:
    default:
        break;
    };
    return "";
}

#include <QString>
enum HOW_TO_DO {
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
        act     = _NONE_ACTION;
        context = _NONE_DO;
        path.clear();
    }
};

#endif // VIRT_ENTITY_ENUMS_H
