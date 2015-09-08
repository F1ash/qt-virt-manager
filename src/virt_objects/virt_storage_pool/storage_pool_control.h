#ifndef STORAGE_POOL_CONTROL_H
#define STORAGE_POOL_CONTROL_H

#include "virt_objects/virt_entity_control.h"
#include "storage_pool_model.h"
#include "storage_pool_toolbar.h"
#include "storage_pool_control_menu.h"
#include "create_widgets/storage/create_pool.h"

class VirtStoragePoolControl : public VirtEntityControl
{
    Q_OBJECT
public:
    explicit VirtStoragePoolControl(QWidget *parent = NULL);
    ~VirtStoragePoolControl();

signals:
    void                       overviewStPool(virConnect*, QString&, QString&);

private:
    StoragePoolModel          *storagePoolModel;
    StoragePoolToolBar        *toolBar;

public slots:
    void                       stopProcessing();
    bool                       setCurrentWorkConnect(virConnect*);
    void                       setListHeader(QString&);
    void                       resultReceiver(Result);
    void                       stopOverView();

private slots:
    void                       reloadState();
    void                       changeDockVisibility();
    void                       entityClicked(const QPoint&);
    void                       entityDoubleClicked(const QModelIndex&);
    void                       execAction(const QStringList&);
    void                       newVirtEntityFromXML(const QStringList&);
    void                       doneEntityCreationDialog();
};

#endif // STORAGE_POOL_CONTROL_H
