#ifndef DOMAIN_CONTROL_H
#define DOMAIN_CONTROL_H

#include "virt_objects/virt_entity_control.h"
#include "domain_model.h"
#include "domain_toolbar.h"
#include "domain_control_menu.h"
#include "create_widgets/domain/create_virt_domain.h"
#include "vm_viewer/vm_viewer.h"
#include "migrate_dialog.h"
#include "domain_list_delegate.h"
#include "create_widgets/snapshot/create_snapshot_dialog.h"
#include "create_widgets/snapshot/snapshot_action_dialog.h"

class VirtDomainControl : public VirtEntityControl
{
    Q_OBJECT
public:
    explicit VirtDomainControl(QWidget *parent = nullptr);
    ~VirtDomainControl();

signals:
    void                     displayRequest(TASK);
    void                     addToStateMonitor(virConnectPtr*, QString&, QString&);
    void                     migrateToConnect(TASK);
    void                     domainToEditor(TASK);

private:
    DomainModel             *domainModel;
    DomainToolBar           *toolBar;
    DomainListDelegate      *domListDlg;

public slots:
    void                     stopProcessing();
    bool                     setCurrentWorkConnect(virConnectPtr*);
    void                     setListHeader(QString&);
    virConnectPtr*           getPtr_connectionPtr();
    void                     execMigrateAction(virConnectPtr*, TASK);
    void                     resultReceiver(Result);

private slots:
    void                     reloadState();
    void                     changeDockVisibility();
    void                     entityClicked(const QPoint&);
    void                     entityDoubleClicked(const QModelIndex&);
    void                     execAction(const QStringList&);
    void                     newVirtEntityFromXML(const QStringList&);
};

#endif // DOMAIN_CONTROL_H
