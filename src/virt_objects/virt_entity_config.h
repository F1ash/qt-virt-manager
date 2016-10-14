#ifndef VIRT_ENTITY_CONFIG
#define VIRT_ENTITY_CONFIG

#include <libvirt.h>
#include <QStringList>
#include <QVariantMap>
#include "virt_entity_enums.h"

typedef QList<QVariantMap> ACT_RESULT;

struct Result {
    QString         name        = QString();
    uint            number      = 0;
    VIRT_ENTITY     type        = _NONE_ENTITY;
    Actions         action      = _NONE_ACTION;
    bool            result      = false;
    QStringList     msg         = QStringList();
    ACT_RESULT      data        = ACT_RESULT();
    QString         err         = QString();
    QString         fileName    = QString();
};

struct TASK {
    // base parameters
    virConnectPtr*      srcConnPtr  = nullptr;
    QString             srcConName  = QString();
    QString             object      = QString();
    Actions             action      = _NONE_ACTION;
    Methods             method      = _NONE_METHOD;
    VIRT_ENTITY         type        = _NONE_ENTITY;

    // details
    struct          DETAILS {
        virConnectPtr*  dstConnPtr  = nullptr;
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
    }                   args;

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

