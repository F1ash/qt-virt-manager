#ifndef VIRT_ENTITY_CONFIG
#define VIRT_ENTITY_CONFIG

#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>
#include <QStringList>

enum Actions {
    GET_ALL_ENTITY_STATE,
    GET_ALL_ENTITY_DATA,
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
    CREATE_DOMAIN_SNAPSHOT,
    REVERT_TO_DOMAIN_SNAPSHOT,
    DELETE_DOMAIN_SNAPSHOT,
    IFACE_CHANGE_BEGIN,
    IFACE_CHANGE_COMMIT,
    IFACE_CHANGE_ROLLBACK,
    _EMPTY_ACTION
};

struct Result {
    QString         name        = QString();
    uint            number      = 0;
    QString         type        = QString();
    Actions         action      = _EMPTY_ACTION;
    bool            result      = false;
    QStringList     msg         = QStringList();
    QString         err         = QString();
    QString         fileName    = QString();
};

struct TASK {
    // base parameters
    virConnectPtr*      srcConnPtr  = NULL;
    QString             srcConName  = QString();
    QString             object      = QString();
    Actions             action      = _EMPTY_ACTION;
    QString             method      = QString();
    QString             type        = QString();

    // details
    struct          DETAILS {
        virConnectPtr*  destConnPtr = NULL;
        qulonglong      offset      = 0;
        qulonglong      size        = 0;
        uint            sign        = 0;
        QString         path        = QString();
        QString         state       = QString();
        QString         object      = QString();
        QString         list()        {
            return QString("%1, %2, %3, %4, %5, %6")
                .arg(offset).arg(size).arg(sign)
                .arg(path).arg(state).arg(object);
        }
    };
    DETAILS             args;

    // secret (clear before deletion)
    class           SECRET {
    ~SECRET() {
        secretValue.clear();
    }
    private:
        QByteArray secretValue;
    public:
        void setSecretValue(QByteArray value) {
            secretValue = value;
        }
        QByteArray getSecretValue() const {
            return secretValue;
        }
    };
    SECRET *secret = new SECRET();
};

#define DFR QString("<||>")

#endif // VIRT_ENTITY_CONFIG

