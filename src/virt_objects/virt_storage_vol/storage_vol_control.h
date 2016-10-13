#ifndef STORAGE_VOL_CONTROL_H
#define STORAGE_VOL_CONTROL_H

#include "virt_objects/virt_entity_control.h"
#include "storage_vol_model.h"
#include "storage_vol_toolbar.h"
#include "storage_vol_control_menu.h"
#include "resize_dialog.h"
#include "create_widgets/storage/create_volume.h"
#include <QCloseEvent>

class VirtStorageVolControl : public VirtEntityControl
{
    Q_OBJECT
public:
    explicit VirtStorageVolControl(QWidget *parent = nullptr);
    ~VirtStorageVolControl();
    QString             getCurrentVolumeName() const;
    QString             getCurrentVolumePath() const;

signals:
    void                finished(const QString&);
    void                volumeToEditor(TASK*);

private:
    QString             currPoolName;
    StorageVolModel    *storageVolModel;
    StorageVolToolBar  *toolBar;

public slots:
    void                stopProcessing();
    bool                setCurrentStoragePool(virConnectPtr*, const QString&, const QString&);
    void                resultReceiver(Result*);
    void                closeEvent(QCloseEvent*);

private slots:
    void                reloadState();
    void                changeDockVisibility();
    void                entityClicked(const QPoint&);
    void                entityDoubleClicked(const QModelIndex&);
    void                execAction(const Act_Param&);
    void                newVirtEntityFromXML(const Act_Param&);

};

#endif // STORAGE_VOL_CONTROL_H
