#ifndef _USE_ENCRYPTION_H
#define _USE_ENCRYPTION_H

#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "create_widgets/storage/_create_storage_widgets/find_secret_dialog.h"
#include "create_widgets/common/_changed.h"

class _UseEncryption : public _Changed
{
    Q_OBJECT
public:
    explicit _UseEncryption(
            QWidget        *parent      = Q_NULLPTR,
            virConnectPtr*  connPtrPtr  = Q_NULLPTR);

signals:
    void                 errorMsg(const QString&);

private:
    QString              currVolumePath;
    QCheckBox           *usage;
    QLineEdit           *secUsage;
    QPushButton         *findSecret;
    QHBoxLayout         *baseLayout;
    QWidget             *baseWdg;
    QLabel              *info;
    QVBoxLayout         *commonLayout;

    virConnectPtr*       ptr_ConnPtr;
    SecretControlThread *hlpThread;

public slots:
    bool                 isUsed() const;
    void                 setUsage(bool);
    void                 setCurrVolumePath(const QString&);
    QString              getSecretUUID() const;
    void                 setSecretUUID(const QString&);

private slots:
    void                 searchInSecretList();
    void                 resultReceiver(Result);
    void                 setVolumeSecret();
    void                 emitCompleteSignal();
};

#endif // _USE_ENCRYPTION_H
