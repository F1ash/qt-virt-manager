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

class _UseEncryption : public QWidget
{
    Q_OBJECT
public:
    explicit _UseEncryption(
            QWidget         *parent = NULL,
            virConnectPtr    _conn  = NULL);

signals:
    void                 dataChanged();

private:
    QString              currVolumePath;
    QCheckBox           *usage;
    QLineEdit           *secUsage;
    QPushButton         *findSecret;
    QHBoxLayout         *baseLayout;
    QWidget             *baseWdg;
    QLabel              *info;
    QVBoxLayout         *commonLayout;

    virConnectPtr        currWorkConnection;
    SecretControlThread *thread;

public slots:
    bool                 isUsed() const;
    void                 setUsage(bool);
    void                 emitSecretList();
    void                 setCurrVolumePath(const QString&);
    QString              getSecretUUID() const;
    void                 setSecretUUID(const QString&);

private slots:
    void                 resultReceiver(Result);
    void                 setVolumeSecret();
};

#endif // _USE_ENCRYPTION_H
