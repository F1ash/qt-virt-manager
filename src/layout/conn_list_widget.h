#ifndef CONN_LIST_WIDGET_H
#define CONN_LIST_WIDGET_H

#include <QTreeView>
#include <QMessageBox>
#include <QInputDialog>
#include "conn_menu.h"
#include "progressbar_delegate.h"
#include "search_thread.h"
#include "wait_local_conn.h"
#include <QDebug>

#define TO_RUN true
#define TO_STOP false

class ConnectionList : public QTreeView
{
    Q_OBJECT
public:
    explicit ConnectionList(QWidget *parent = NULL);
    CONN_LIST           *connections;
    ConnItemModel       *connItemModel;

signals:
    void                 clickedItem(QString);
    void                 removeConnection(QString&);
    void                 messageShowed();
    void                 warning(QString&);
    void                 connPtr(virConnectPtr*, QString&);
    void                 connToClose(int);
    void                 connClosed(bool, QString&);
    void                 domResult(Result);
    void                 netResult(Result);

public slots:
    int                  connItemEditAction();
    void                 searchLocalhostConnections();
    void                 refreshLocalhostConnection();
    void                 addConnItem(QString&);
    void                 deleteCurrentConnection();
    void                 openConnection(QModelIndex&);
    void                 showConnection(QModelIndex&);
    void                 closeConnection(QModelIndex&);
    virConnectPtr       *getConnectionPtr(QString&);
    void                 stopProcessing();

private :
    ConnSettings        *sDialog;
    ProgressBarDelegate *progressBarDlg;
    SearchThread        *searchThread;
    WaitLocalConn       *waitLocalConn;
    int                  localConn = 0;

private slots:
    void                 connItemClicked(const QPoint&);
    void                 connItemDoubleClicked(const QModelIndex&);
    void                 connItemKillAction();
    void                 connItemRunAction();
    void                 connItemShowAction();
    void                 createConnection(QModelIndex&);
    void                 createLocalConnection(QString&);
    void                 checkConnection(QModelIndex&, bool);
    void                 deleteCancelledCreation();
    void                 showMessage(QString, QString);
    void                 sendWarning(QString&);
    void                 mainWindowUp();
    void                 sendConnPtr(virConnectPtr*, QString&);
    void                 getAuthCredentials(QString&);
};

#endif   // CONN_LIST_WIDGET_H
