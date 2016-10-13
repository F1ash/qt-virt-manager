#ifndef CONN_LIST_WIDGET_H
#define CONN_LIST_WIDGET_H

#include "virt_objects/tree_view.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QMutex>
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

    void                 setUsageInSoftTouched(bool);
    int                  getListItemCount() const;
    QModelIndex          getCustomIndex(int) const;
    ConnItemIndex*       getConnItemDataListIndex(int) const;
    int                  getConnItemDataListCount() const;
    int                  getConnItemDataListIndexOf(ConnItemIndex*) const;
    bool                 removeListItem(int) const;
    bool                 removeConnItemDataList(ConnItemIndex*) const;
    ConnElement*         getConnElementByName(QString) const;
    int                  removeConnectionItembyName(QString) const;
    void                 addConnItem(const QString&);
    void                 openConnection(const QModelIndex&);
    void                 overviewOfConnection(const QModelIndex&);
    void                 closeConnection(const QModelIndex&);
    virConnectPtr*       getPtr_connectionPtr(const QString&);

signals:
    void                 clickedItem(const QString&);
    void                 removeConnection(const QString&);
    void                 messageShowed();
    void                 warning(const QString&);
    void                 connToOverview(virConnectPtr*, const QString&);
    void                 connClosed(bool, const QString&);
    void                 domResult(Result*);
    void                 netResult(Result*);
    void                 domainEnd(const QString&);
    void                 searchStarted();
    void                 searchFinished();

public slots:
    void                 refreshLocalhostConnection();
    int                  connItemEditAction();
    void                 deleteCurrentConnection();
    void                 stopProcessing();

private :
    QMutex               mutex;
    CONN_LIST           *connections;
    ConnItemModel       *connItemModel;
    ConnListDelegate    *connListDlg;
    SearchThread        *searchThread;
    WaitLocalConn       *waitLocalConn;
    int                  localConn = 0;

    void                 createConnection(const QModelIndex&);
    void                 checkConnection(const QModelIndex&, const bool);
    void                 deleteCurrentConnection(const QModelIndex&);
    void                 showMessage(const QString&, const QString&);
    bool                 onViewExist;

private slots:
    void                 searchLocalhostConnections();
    void                 connContextMenuRequested(const QPoint&);
    void                 connItemClicked(const QModelIndex&);
    void                 connItemDoubleClicked(const QModelIndex&);
    void                 connItemKillAction();
    void                 connItemRunAction();
    void                 connItemShowAction();
    void                 createLocalConnection(const QString&);
    void                 deleteCancelledCreation();
    void                 sendWarning(const QString&);
    void                 sendWarning(const QString&, const uint);
    void                 mainWindowUp();
    void                 getAuthCredentials(const QString&);
    void                 searchLocalhostConnComplete();
    void                 setOnViewAvailableConnection(const QString&);
    void                 newItemClosed(bool);
    void                 emitDomainStateChanged(Result);
    void                 emitNetworkStateChanged(Result);
};

#endif   // CONN_LIST_WIDGET_H
