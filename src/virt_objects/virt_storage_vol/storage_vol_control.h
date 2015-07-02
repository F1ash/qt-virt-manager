#ifndef STORAGE_VOL_CONTROL_H
#define STORAGE_VOL_CONTROL_H

#include "virt_objects/virt_entity_control.h"
#include "storage_vol_model.h"
#include "storage_vol_toolbar.h"
#include "storage_vol_control_menu.h"
#include "resize_dialog.h"
#include "create_widgets/storage/create_volume.h"

class VirtStorageVolControl : public VirtEntityControl
{
    Q_OBJECT
public:
    explicit VirtStorageVolControl(QWidget *parent = NULL);
    ~VirtStorageVolControl();

private:
    QString                    currPoolName;
    StorageVolModel           *storageVolModel = NULL;
    StorageVolToolBar         *toolBar;

public slots:
    bool                       getThreadState() const;
    void                       stopProcessing();
    bool                       setCurrentStoragePool(virConnect*, QString&, QString&);
    QString                    getCurrentVolumeName() const;
    void                       resultReceiver(Result);

private slots:
    void                       reloadState();
    void                       changeDockVisibility();
    void                       entityClicked(const QPoint&);
    void                       entityDoubleClicked(const QModelIndex&);
    void                       execAction(const QStringList&);
    void                       newVirtEntityFromXML(const QStringList&);

};

#endif // STORAGE_VOL_CONTROL_H
