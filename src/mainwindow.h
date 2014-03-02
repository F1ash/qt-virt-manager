#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QTimer>
#include <QSettings>
#include <QCloseEvent>
#include "log_doc/log_doc.h"
#include "virt_objects/virt_network/virtnet_control.h"
#include "virt_objects/virt_domain/domain_doc.h"
#include "virt_objects/virt_storage_vol/storage_vol_doc.h"
#include "virt_objects/virt_storage_pool/storage_pool_doc.h"
#include "tray/traywidget.h"
#include "layout/conn_list_widget.h"
#include "toolbar/toolbar.h"
#include "wait_thread/wait_thread.h"
#include <QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

public slots:

private :
    QSettings               settings;
    ConnectList            *connListWidget;
    TrayIcon               *trayIcon;
    ToolBar                *toolBar;
    Wait                   *wait_thread = NULL;
    QDockWidget            *logDoc;
    LogDocContent          *logDocContent;
    QDockWidget            *domainDoc;
    DomainDocContent       *domainDocContent;
    QDockWidget            *networkDoc;
    VirtNetControl         *networkDocContent;
    QDockWidget            *storageVolDoc;
    StorageVolDocContent   *storageVolDocContent;
    QDockWidget            *storagePoolDoc;
    StoragePoolDocContent  *storagePoolDocContent;

private slots:
    void closeEvent(QCloseEvent*);
    void closeEvent();
    void changeVisibility();
    void mainWindowUp();
    void initDockWidgets();
    void initTrayIcon();
    void trayIconActivated(QSystemTrayIcon::ActivationReason);
    void initConnListWidget();
    void initToolBar();
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
    Qt::DockWidgetArea getDocArea(int) const;
    void receiveConnPtr(virConnect*);
    void stopProcessing();

};

#endif // MAINWINDOW_H
