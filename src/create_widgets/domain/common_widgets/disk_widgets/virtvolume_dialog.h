#ifndef VIRTVOLUME_DIALOG_H
#define VIRTVOLUME_DIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include "virt_objects/virt_storage_vol/storage_vol_control.h"
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include <QDebug>

class VirtVolumeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VirtVolumeDialog(
            QWidget *parent = NULL,
            virConnectPtr conn = NULL);

signals:

private:
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
    virConnectPtr    currWorkConnect = NULL;

public slots:
    QStringList getResult() const;

private slots:
    void setPoolList();
    void set_Result();
    void showVolumes(QListWidgetItem*);
    void showMsg(QString&);
};

#endif // VIRTVOLUME_DIALOG_H
