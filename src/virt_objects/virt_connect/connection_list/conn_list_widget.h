#ifndef CONN_LIST_WIDGET_H
#define CONN_LIST_WIDGET_H

#include "virt_objects/tree_view.h"
#include <QMessageBox>
#include <QInputDialog>
#include "conn_menu.h"
#include "conn_list_delegate.h"
#include "search_thread.h"
#include "wait_local_conn.h"
#include <QDebug>

#define TO_RUN true
#define TO_STOP false

class ConnectionList : public TreeView
{
    Q_OBJECT
public:
    explicit ConnectionList(QWidget *parent = nullptr);
    CONN_LIST           *connections;
    ConnItemModel       *connItemModel;

    void                 setUsageInSoftTouched(bool);

signals:
    void                 clickedItem(QString);
    void                 removeConnection(QString&);
    void                 messageShowed();
    void                 warning(QString&);
    void                 connPtrPtr(virConnectPtr*, QString&);
    void                 connToClose(int);
    void                 connClosed(bool, QString&);
    void                 domResult(Result);
    void                 netResult(Result);
    void                 domainEnd(QString&);

public slots:
    int                  connItemEditAction();
    void                 refreshLocalhostConnection();
    void                 addConnItem(QString&);
    void                 deleteCurrentConnection();
    void                 openConnection(const QModelIndex&);
    void                 showConnection(const QModelIndex&);
    void                 closeConnection(const QModelIndex&);
    virConnectPtr       *getPtr_connectionPtr(const QString&);
    void                 stopProcessing();

private :
    ConnListDelegate    *connListDlg;
    SearchThread        *searchThread;
    WaitLocalConn       *waitLocalConn;
    int                  localConn = 0;

private slots:
    void                 searchLocalhostConnections();
    void                 connContextMenuRequested(const QPoint&);
    void                 connItemDoubleClicked(const QModelIndex&);
    void                 connItemKillAction();
    void                 connItemRunAction();
    void                 connItemShowAction();
    void                 createConnection(QModelIndex&);
    void                 createLocalConnection(QString&);
    void                 checkConnection(const QModelIndex&, bool);
    void                 deleteCancelledCreation();
    void                 deleteCurrentConnection(QModelIndex&);
    void                 showMessage(QString, QString);
    void                 sendWarning(QString&);
    void                 sendWarning(QString&, uint);
    void                 mainWindowUp();
    void                 sendConnPtrPtr(virConnectPtr*, QString&);
    void                 getAuthCredentials(QString&);
    void                 searchLocalhostConnComplete();
};

#endif   // CONN_LIST_WIDGET_H
