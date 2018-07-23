#ifndef STORAGE_POOL_CONTROL_H
#define STORAGE_POOL_CONTROL_H

#include "virt_objects/virt_entity_control.h"
#include "storage_pool_model.h"
#include "storage_pool_toolbar.h"
#include "storage_pool_control_menu.h"
#include "storage_pool_list_delegate.h"
#include "create_widgets/storage/create_pool.h"

class VirtStoragePoolControl : public VirtEntityControl
{
    Q_OBJECT
public:
    explicit VirtStoragePoolControl(QWidget *parent = Q_NULLPTR);
    ~VirtStoragePoolControl();

signals:
    void                       overviewStPool(virConnectPtr*, const QString&, const QString&);

private:
    StoragePoolModel          *storagePoolModel;
    StoragePoolToolBar        *toolBar;
    StoragePoolListDelegate   *storagePoolListDlg;

public slots:
    void                       stopProcessing();
    bool                       setCurrentWorkConnection(virConnectPtr*);
    void                       setListHeader(const QString&);
    void                       resultReceiver(Result*);
    void                       stopOverView();
    void                       reloadState();

private slots:
    void                       changeDockVisibility();
    void                       entityClicked(const QPoint&);
    void                       entityDoubleClicked(const QModelIndex&);
    void                       execAction(const Act_Param&);
    void                       newVirtEntityFromXML(const Act_Param&);
};

#endif // STORAGE_POOL_CONTROL_H
