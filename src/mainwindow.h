#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTimerEvent>
#include <QSettings>
#include <QCloseEvent>
#include <QHBoxLayout>
#include "log_dock/log_dock.h"
#include "virt_objects/virt_event_loop.h"
#include "virt_objects/virt_connect/connection_list/connections.h"
#include "virt_objects/virt_network/virtnet_control.h"
#include "virt_objects/virt_domain/domain_control.h"
#include "virt_objects/virt_storage_vol/storage_vol_control.h"
#include "virt_objects/virt_storage_pool/storage_pool_control.h"
#include "virt_objects/virt_secret/secret_control.h"
#include "virt_objects/virt_interface/virt_interface_control.h"
#include "tray/traywidget.h"
#include "wait_thread/wait_thread.h"
#include "vm_viewer/lxc/lxc_viewer.h"
#include "vm_viewer/spice/spice_viewer.h"
#include "vm_viewer/vnc/vnc_viewer.h"
#include "state_monitor/domain_state_monitor.h"
#include "task_warehouse/task_warehouse.h"
#include "dock_head_widgets/dock_widget.h"
#include "dock_head_widgets/dock_head_widget.h"
#include "menubar/menu_bar.h"
#include "proxy_widget/proxy_widget.h"
#include "soft_touched/st_stacked_widget.h"

typedef QMap<QString, VM_Viewer*> ViewerMap;
typedef QMap<QString, VirtStorageVolControl*> OverviwedStorageMap;
typedef QMap<QString, CreateVirtDomain*> DomainEditorMap;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:

public slots:

private :
    QSettings                    settings;
    ProxyWidget                 *proxyWdg;
    QHBoxLayout                 *proxyLayout;
    ST_StackedWidget            *SoftTouchedWdg;
    Connections                 *connListWidget;
    TrayIcon                    *trayIcon;
    MenuBar                     *menuBar;
    Wait                        *wait_thread = nullptr;
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
    DockWidget                  *ifaceDock;
    VirtInterfaceControl        *ifaceDockContent;
    DomainStateMonitor          *domainsStateMonitor;
    TaskWareHouse               *taskWrHouse;

    DockHeadWidget              *logHeadWdg;
    DockHeadWidget              *domHeadWdg;
    DockHeadWidget              *netHeadWdg;
    DockHeadWidget              *poolHeadWdg;
    DockHeadWidget              *scrtHeadWdg;
    DockHeadWidget              *ifaceHeadWdg;

    QProgressBar                *closeProgress;
    int                          killTimerId = 0;
    int                          counter = 0;
    int                          waitAtClose;
    bool                         reloadFlag = false;
    VIEW_MODE                    viewMode;

    ViewerMap                    VM_Displayed_Map;
    OverviwedStorageMap          Overviewed_StPool_Map;
    DomainEditorMap              DomainEditor_Map;

    VirtEventLoop               *virtEventLoop;

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
    void initMenuBar();
    void initConnListWidget();
    void initDockWidgets();
    void initVirEventloop();
    void virtEventLoopFinished();
    void restartApplication();
    void initConnections(bool);
    void editCurrentConnection();
    void createNewConnection();
    void deleteCurrentConnection();
    void removeConnItem(QString&);
    void openCurrentConnection();
    void showCurrentConnection();
    void closeCurrentConnection();
    void closeConnection(int);
    void closeAllConnections();
    void closeConnGenerations(int);
    void closeConnGenerations(QString&);
    bool runningConnExist();
    void autoHide();
    void writeToErrorLog(QString&);
    void writeToErrorLog(QString&, uint);
    Qt::DockWidgetArea getDockArea(int) const;
    void receiveConnPtrPtr(virConnectPtr*, QString&);
    void stopConnProcessing(bool, QString &);
    void stopProcessing();
    void invokeVMDisplay(TASK);
    void deleteVMDisplay(QString&);
    void buildMigrateArgs(TASK);
    void overviewStoragePool(virConnectPtr*, QString&, QString&);
    void deleteStPoolOverview(QString&);
    void invokeDomainEditor(TASK);
    void deleteDomainEditor(QString&);
    void migrate_settings_to_INI_format();

    void free_and_hide_all_stuff();
    void all_stuff_to_original();
    void setDockFloatible(bool);
    void setDockHeaderWheelEventsEnabled(bool);
    void setDockUsedInSoftTouched(bool);
    void turnSoftTouchedToUntriggered();
};

#endif // MAINWINDOW_H
