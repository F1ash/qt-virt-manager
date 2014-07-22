#ifndef DOMAIN_CONTROL_H
#define DOMAIN_CONTROL_H

#include <QMainWindow>
#include <QGridLayout>
#include <QTreeView>
#include <QSettings>
#include <QTime>
#include <QUrl>
#include <QDesktopServices>
#include <QDebug>
#include "domain_model.h"
#include "domain_toolbar.h"
#include "domain_control_menu.h"
#include "domain_control_thread.h"
#include "create_widgets/domain/create_virt_domain.h"
#include "vm_viewer/vm_viewer.h"
#include "migrate_dialog.h"

class VirtDomainControl : public QMainWindow
{
    Q_OBJECT
public:
    explicit VirtDomainControl(QWidget *parent = 0);
    ~VirtDomainControl();

signals:
    void domMsg(QString&);
    void displayRequest(virConnect*, QString, QString);
    void domainClosed(QString, QString);
    void migrateToConnect(QStringList&);

private:
    QString                  currConnName;
    QSettings                settings;
    DomainModel             *domainModel = NULL;
    QTreeView               *domainList  = NULL;
    DomainToolBar           *toolBar;

    virConnect              *currWorkConnect  = NULL;
    DomControlThread        *domControlThread = NULL;
    CreateVirtDomain        *createVirtDomain = NULL;

public slots:
    bool                     getThreadState() const;
    void                     stopProcessing();
    bool                     setCurrentWorkConnect(virConnect*);
    void                     setListHeader(QString&);
    virConnect*              getConnect() const;
    void                     execMigrateAction(virConnectPtr, QStringList&);
    void                     reloadDomainState();

private slots:
    void                     resultReceiver(DomActions, Result);
    void                     msgRepeater(QString);
    void                     changeDockVisibility();

    void                     domainClicked(const QPoint&);
    void                     domainDoubleClicked(const QModelIndex&);
    void                     execAction(const QStringList&);
    void                     newVirtDomainFromXML(const QStringList&);

};

#endif // DOMAIN_CONTROL_H
