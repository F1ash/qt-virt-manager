#ifndef VIRTVOLUME_DIALOG_H
#define VIRTVOLUME_DIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include "virt_objects/virt_storage_vol/storage_vol_control.h"
#include "virt_objects/virt_storage_vol/storage_vol_control_thread.h"
#include "virt_objects/virt_storage_pool/storage_pool_control_thread.h"
#include <QDebug>

struct VVD_Result {
    QString pool    = QString();
    QString type    = QString();
    QString name    = QString();
    QString path    = QString();
    QString source  = QString();
    QString target  = QString();
};

class VirtVolumeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VirtVolumeDialog(
            QWidget        *parent  = NULL,
            virConnectPtr*  connPtr = NULL,
            QString         _type   = QString());

signals:

private:
    const QString    type;
    QPushButton     *chooseVolume;
    QPushButton     *cancel;
    QHBoxLayout     *buttonLayout;
    QWidget         *buttons;

    QListWidget     *poolList;
    VirtStorageVolControl
                    *volumes;
    QHBoxLayout     *listLayout;
    QWidget         *listWidget;
    QVBoxLayout     *commonLayout;
    virConnectPtr*   ptr_ConnPtr;
    StorageVolControlThread
                    *storageVolThread;
    StoragePoolControlThread
                    *storagePoolThread;

public slots:
    VVD_Result       getResult() const;

private slots:
    void             setPoolList();
    void             set_Result();
    void             showVolumes(QListWidgetItem*);
    void             showMsg(QString&);
    void             execAction(TASK);
    void             poolThreadResult(Result);
    void             addPoolItem(QStringList&);
};

#endif // VIRTVOLUME_DIALOG_H
