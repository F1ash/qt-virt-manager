#ifndef STORAGE_POOL_CONTROL_H
#define STORAGE_POOL_CONTROL_H

#include <QMainWindow>
#include <QGridLayout>
#include <QTreeView>
#include <QSettings>
#include <QTime>
#include <QUrl>
#include <QDesktopServices>
#include <QMessageBox>
#include <QDebug>
#include "storage_pool_model.h"
#include "storage_pool_toolbar.h"
#include "storage_pool_control_menu.h"
#include "storage_pool_control_thread.h"
#include "create_widgets/storage/create_pool.h"

class VirtStoragePoolControl : public QMainWindow
{
    Q_OBJECT
public:
    explicit VirtStoragePoolControl(QWidget *parent = NULL);
    ~VirtStoragePoolControl();

signals:
    void storagePoolMsg(QString&);
    void currPool(virConnect*, QString&, QString&);

private:
    QString                    currConnName;
    QSettings                  settings;
    StoragePoolModel          *storagePoolModel = NULL;
    QTreeView                 *storagePoolList = NULL;
    StoragePoolToolBar        *toolBar;

    virConnect                *currWorkConnect = NULL;
    StoragePoolControlThread  *stPoolControlThread = NULL;

public slots:
    bool getThreadState() const;
    void stopProcessing();
    bool setCurrentWorkConnect(virConnect*);
    void setListHeader(QString&);

private slots:
    void resultReceiver(StoragePoolActions, QStringList);
    void msgRepeater(QString);
    void changeDockVisibility();

    void storagePoolClicked(const QPoint&);
    void storagePoolDoubleClicked(const QModelIndex&);
    void execAction(const QStringList&);
    void newVirtStoragePoolFromXML(const QStringList&);

};

#endif // STORAGE_POOL_CONTROL_H
