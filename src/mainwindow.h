#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QTimer>
#include <QSettings>
#include <QCloseEvent>
#include "log_dock/log_dock.h"
#include "virt_objects/virt_network/virtnet_control.h"
#include "virt_objects/virt_domain/domain_control.h"
#include "virt_objects/virt_storage_vol/storage_vol_control.h"
#include "virt_objects/virt_storage_pool/storage_pool_control.h"
#include "tray/traywidget.h"
#include "layout/conn_list_widget.h"
#include "toolbar/toolbar.h"
#include "wait_thread/wait_thread.h"
#include "vm_viewer/vm_viewer.h"
#include "state_monitor/domain_state_monitor.h"
#include <QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = NULL);
    ~MainWindow();

signals:

public slots:

private :
    QSettings                    settings;
    ConnectList                 *connListWidget;
    TrayIcon                    *trayIcon;
    ToolBar                     *toolBar;
    Wait                        *wait_thread = NULL;
    QDockWidget                 *logDock;
    LogDock                     *logDockContent;
    QDockWidget                 *domainDock;
    VirtDomainControl           *domainDockContent;
    QDockWidget                 *networkDock;
    VirtNetControl              *networkDockContent;
    QDockWidget                 *storageVolDock;
    VirtStorageVolControl       *storageVolDockContent;
    QDockWidget                 *storagePoolDock;
    VirtStoragePoolControl      *storagePoolDockContent;
    ViewerMap                    VM_Displayed_Map;
    DomainStateMonitor          *domainsStateMonitor;

private slots:
    void closeEvent(QCloseEvent*);
    void closeEvent();
    void changeVisibility();
    void mainWindowUp();
    void initDomainStateMonitor();
    void initTrayIcon();
    void trayIconActivated(QSystemTrayIcon::ActivationReason);
    void initConnListWidget();
    void initToolBar();
    void initDockWidgets();
    void editCurrentConnect();
    void createNewConnect();
    void deleteCurrentConnect();
    void removeConnectItem(QString&);
    void openCurrentConnect();
    void showCurrentConnect();
    void closeCurrentConnect();
    void closeConnect(int);
    void closeAllConnect();
    bool runningConnectsExist();
    void autoHide();
    void writeToErrorLog(QString&);
    void changeLogViewerVisibility();
    Qt::DockWidgetArea getDockArea(int) const;
    void receiveConnPtr(virConnect*, QString&);
    void receivePoolName(virConnect*, QString&, QString&);
    void stopConnProcessing(virConnect*);
    void stopProcessing();
    void invokeVMDisplay(virConnect*, QString, QString);
    void deleteVMDisplay();
    void deleteVMDisplay(QString, QString);
    void buildMigrateArgs(QStringList&);

};

#endif // MAINWINDOW_H
