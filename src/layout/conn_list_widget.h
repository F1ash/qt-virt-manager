#ifndef CONN_LIST_WIDGET_H
#define CONN_LIST_WIDGET_H

#include <QTreeView>
#include <QMessageBox>
#include <QInputDialog>
#include "conn_menu.h"
#include "conn_item_model.h"
#include "progressbar_delegate.h"
#include "search_thread.h"
#include "virt_objects/virt_connect/conn_element.h"
#include <QDebug>

#define TO_RUN true
#define TO_STOP false
typedef QMap<QString, ConnElement*> CONN_LIST;

class ConnectionList : public QTreeView
{
    Q_OBJECT
public:
    ConnectionList(QWidget *parent);
    ~ConnectionList();

    CONN_LIST           *connections;
    ConnItemModel       *connItemModel;

signals:
    void clickedItem(QString);
    void removeConnection(QString&);
    void messageShowed();
    void warning(QString&);
    void connPtr(virConnect*, QString&);
    void connectionClosed(virConnect*);
    void domResult(Result);

public slots:
    int  connItemEditAction();
    void searchLocalhostConnections();
    void addConnItem(QString&);
    void deleteCurrentConnection();
    void openConnection(QModelIndex&);
    void showConnection(QModelIndex&);
    void closeConnection(QModelIndex&);
    virConnectPtr getConnection(QString&);
    void stopProcessing();

private :
    ConnSettings         *sDialog;
    ProgressBarDelegate  *progressBarDlg;
    SearchThread         *searchThread;

private slots:
    void connItemClicked(const QPoint&);
    void connItemDoubleClicked(const QModelIndex&);
    void connItemKillAction();
    void connItemRunAction();
    void connItemShowAction();
    void createConnection(QModelIndex&);
    void checkConnection(QModelIndex&, bool);
    void compareConnURI(QString&);
    void deleteCancelledCreation();
    void showMessage(QString, QString);
    void sendWarning(QString&);
    void mainWindowUp();
    void sendConnPtr(virConnect*, QString&);
    void getAuthCredentials(QString&);
};

#endif   // CONN_LIST_WIDGET_H
