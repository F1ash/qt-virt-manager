#ifndef DOMAIN_CONTROL_H
#define DOMAIN_CONTROL_H

#include "virt_objects/virt_entity_control.h"
#include "domain_model.h"
#include "domain_toolbar.h"
#include "domain_control_menu.h"
#include "create_widgets/domain/create_virt_domain.h"
#include "vm_viewer/vm_viewer.h"
#include "migrate_dialog.h"

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
    void                     migrateToConnect(QStringList&);

private:
    DomainModel             *domainModel = NULL;
    DomainToolBar           *toolBar;
    CreateVirtDomain        *createVirtDomain = NULL;

public slots:
    bool                     getThreadState() const;
    void                     stopProcessing();
    bool                     setCurrentWorkConnect(virConnect*);
    void                     setListHeader(QString&);
    virConnect*              getConnect() const;
    void                     execMigrateAction(virConnectPtr, QStringList&);
    void                     reloadDomainState();
    void                     resultReceiver(Result);

private slots:
    void                     changeDockVisibility();
    void                     entityClicked(const QPoint&);
    void                     entityDoubleClicked(const QModelIndex&);
    void                     execAction(const QStringList&);
    void                     newVirtEntityFromXML(const QStringList&);
};

#endif // DOMAIN_CONTROL_H
