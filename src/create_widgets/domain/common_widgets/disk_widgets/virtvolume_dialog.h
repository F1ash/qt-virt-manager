#ifndef VIRTVOLUME_DIALOG_H
#define VIRTVOLUME_DIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "virt_objects/virt_storage_vol/storage_vol_control.h"
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include <QDebug>

class VirtVolumeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VirtVolumeDialog(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

signals:

private:
    QPushButton     *choiceVolume;
    QPushButton     *cancel;
    QHBoxLayout     *buttonlayout;
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
};

#endif // VIRTVOLUME_DIALOG_H
