#ifndef VIRT_ENTITY_CONFIG
#define VIRT_ENTITY_CONFIG

extern "C" {
#include <libvirt.h>
}
#include <QStringList>
#include <QVariantMap>
#include "virt_entity_enums.h"

typedef QList<QVariantMap> ACT_RESULT;

struct Result {
    QString         name        = QString();
    uint            number      = 0;
    VIRT_ENTITY     type        = VIRT_ENTITY::_NONE_ENTITY;
    Actions         action      = Actions::_NONE_ACTION;
    bool            result      = false;
    QStringList     msg         = QStringList();
    ACT_RESULT      data        = ACT_RESULT();
    QString         err         = QString();
    QString         fileName    = QString();
};

struct TASK {
    // base parameters
    virConnectPtr*      srcConnPtr  = Q_NULLPTR;
    QString             srcConName  = QString();
    QString             object      = QString();
    Actions             action      = Actions::_NONE_ACTION;
    Methods             method      = Methods::_NONE_METHOD;
    VIRT_ENTITY         type        = VIRT_ENTITY::_NONE_ENTITY;

    // details
    struct          DETAILS {
        virConnectPtr*  dstConnPtr  = Q_NULLPTR;
        qulonglong      offset      = 0;
        qulonglong      size        = 0;
        int             sign        = 0;
        QString         path        = QString();
        QString         state       = QString();
        QString         object      = QString();
        QString         list()        {
            return QString("%1, %2, %3, %4, %5, %6")
                .arg(offset).arg(size).arg(sign)
                .arg(path).arg(state).arg(object);
        }
    }                   args;

    // secret (clear before deletion)
    class           SECRET {
    ~SECRET() {
        secretValue.clear();
    }
    private:
        QByteArray secretValue;
    public:
        void setSecretValue(QByteArray *value) {
            secretValue = *value;
            value->clear();
        }
        QByteArray getSecretValue() const {
            return secretValue;
        }
    };
    SECRET *secret = new SECRET();
};

#endif // VIRT_ENTITY_CONFIG

