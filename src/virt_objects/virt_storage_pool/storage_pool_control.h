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
    explicit VirtStoragePoolControl(QWidget *parent = nullptr);
    ~VirtStoragePoolControl();

signals:
    void                       overviewStPool(virConnectPtr*, QString&, QString&);
    void                       poolToEditor(TASK);

private:
    StoragePoolModel          *storagePoolModel;
    StoragePoolToolBar        *toolBar;
    StoragePoolListDelegate   *storagePoolListDlg;

public slots:
    void                       stopProcessing();
    bool                       setCurrentWorkConnect(virConnectPtr*);
    void                       setListHeader(QString&);
    void                       resultReceiver(Result);
    void                       stopOverView();

private slots:
    void                       reloadState();
    void                       changeDockVisibility();
    void                       entityClicked(const QPoint&);
    void                       entityDoubleClicked(const QModelIndex&);
    void                       execAction(const QStringList&);
    void                       newVirtEntityFromXML(const OFILE_TASK&);
};

#endif // STORAGE_POOL_CONTROL_H
