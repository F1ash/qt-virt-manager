#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTimerEvent>
#include <QSettings>
#include <QCloseEvent>
#include "log_dock/log_dock.h"
#include "virt_objects/virt_network/virtnet_control.h"
#include "virt_objects/virt_domain/domain_control.h"
#include "virt_objects/virt_storage_vol/storage_vol_control.h"
#include "virt_objects/virt_storage_pool/storage_pool_control.h"
#include "virt_objects/virt_secret/secret_control.h"
#include "tray/traywidget.h"
#include "layout/conn_list_widget.h"
#include "toolbar/toolbar.h"
#include "wait_thread/wait_thread.h"
#include "vm_viewer/lxc/lxc_viewer.h"
#include "vm_viewer/spice/spice_viewer.h"
#include "state_monitor/domain_state_monitor.h"
#include "task_warehouse/task_warehouse.h"
#include "dock_head_widgets/dock_widget.h"
#include "dock_head_widgets/dock_head_widget.h"
#include <QDebug>

typedef QMap<QString, VM_Viewer*> ViewerMap;
typedef QMap<QString, VirtStorageVolControl*> StorageMap;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = NULL);

signals:

public slots:

private :
    QSettings                    settings;
    ConnectionList              *connListWidget;
    TrayIcon                    *trayIcon;
    ToolBar                     *toolBar;
    Wait                        *wait_thread = NULL;
    DockWidget                  *logDock;
    LogDock                     *logDockContent;
    DockWidget                  *domainDock;
    VirtDomainControl           *domainDockContent;
    DockWidget                  *networkDock;
    VirtNetControl              *networkDockContent;
    DockWidget                  *storagePoolDock;
    VirtStoragePoolControl      *storagePoolDockContent;
    DockWidget                  *secretDock;
    VirtSecretControl           *secretDockContent;
    ViewerMap                    VM_Displayed_Map;
    DomainStateMonitor          *domainsStateMonitor;
    TaskWareHouse               *taskWrHouse;

    DockHeadWidget              *logHeadWdg;
    DockHeadWidget              *domHeadWdg;
    DockHeadWidget              *netHeadWdg;
    DockHeadWidget              *poolHeadWdg;
    DockHeadWidget              *scrtHeadWdg;

    QProgressBar                *closeProgress;
    int                          killTimerId = 0;
    int                          counter = 0;
    StorageMap                   storageMap;

private slots:
    void saveSettings();
    void closeEvent(QCloseEvent*);
    void startCloseProcess();
    void timerEvent(QTimerEvent*);
    void changeVisibility();
    void mainWindowUp();
    void initTaskWareHouse();
    void initDomainStateMonitor();
    void initTrayIcon();
    void trayIconActivated(QSystemTrayIcon::ActivationReason);
    void initConnListWidget();
    void initToolBar();
    void initDockWidgets();
    void editCurrentConnection();
    void createNewConnection();
    void deleteCurrentConnection();
    void removeConnItem(QString&);
    void openCurrentConnection();
    void showCurrentConnection();
    void closeCurrentConnection();
    void closeConnection(int);
    void closeAllConnections();
    void closeConnStorageQverview(int);
    bool runningConnExist();
    void autoHide();
    void writeToErrorLog(QString&);
    void changeLogViewerVisibility();
    Qt::DockWidgetArea getDockArea(int) const;
    void receiveConnPtr(virConnect*, QString&);
    void stopConnProcessing(virConnect*);
    void stopProcessing();
    void invokeVMDisplay(virConnect*, QString, QString);
    void deleteVMDisplay(QString&);
    void deleteVMDisplay(QString, QString);
    void buildMigrateArgs(QStringList&);
    void addStorageVol(virConnect*,QString&,QString&);
};

#endif // MAINWINDOW_H
