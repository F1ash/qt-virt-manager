#ifndef VIRT_ENTITY_CONFIG
#define VIRT_ENTITY_CONFIG

#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include <QStringList>

enum Actions {
    GET_ALL_ENTITY,
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
    CHANGE_ENTITY_AUTOSTART,
    GET_XML_DESCRIPTION,
    _EMPTY_ACTION
};

struct Result {
    QString         name   = QString();
    uint            number = 0;
    QString         type   = QString();
    int             action = -1;
    bool            result = false;
    QStringList     msg    = QStringList();
};

#endif // VIRT_ENTITY_CONFIG

