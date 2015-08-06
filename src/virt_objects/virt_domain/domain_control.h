#ifndef DOMAIN_CONTROL_H
#define DOMAIN_CONTROL_H

#include "virt_objects/virt_entity_control.h"
#include "domain_model.h"
#include "domain_toolbar.h"
#include "domain_control_menu.h"
#include "create_widgets/domain/create_virt_domain.h"
#include "vm_viewer/vm_viewer.h"
#include "migrate_dialog.h"
#include "create_widgets/snapshot/create_snapshot_dialog.h"
#include "create_widgets/snapshot/snapshot_action_dialog.h"

class VirtDomainControl : public VirtEntityControl
{
    Q_OBJECT
public:
    explicit VirtDomainControl(QWidget *parent = NULL);
    ~VirtDomainControl();

signals:
    void                     displayRequest(virConnect*, QString, QString);
    void                     addToStateMonitor(virConnectPtr, QString&, QString&);
    void                     domainClosed(QString, QString);
    void                     migrateToConnect(TASK);

private:
    DomainModel             *domainModel = NULL;
    DomainToolBar           *toolBar;

public slots:
    bool                     getThreadState() const;
    void                     stopProcessing();
    bool                     setCurrentWorkConnect(virConnect*);
    void                     setListHeader(QString&);
    virConnect*              getConnection() const;
    void                     execMigrateAction(virConnectPtr, TASK);
    void                     resultReceiver(Result);

private slots:
    void                     reloadState();
    void                     changeDockVisibility();
    void                     entityClicked(const QPoint&);
    void                     entityDoubleClicked(const QModelIndex&);
    void                     execAction(const QStringList&);
    void                     newVirtEntityFromXML(const QStringList&);
    void                     doneEntityCreationDialog();
};

#endif // DOMAIN_CONTROL_H
