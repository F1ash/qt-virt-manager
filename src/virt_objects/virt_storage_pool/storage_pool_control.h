#ifndef STORAGE_POOL_CONTROL_H
#define STORAGE_POOL_CONTROL_H

#include <QMainWindow>
#include <QGridLayout>
#include <QTreeView>
#include <QSettings>
#include <QTime>
#include <QTimerEvent>
#include <QProcess>
#include <QDebug>
#include "storage_pool_model.h"
#include "storage_pool_toolbar.h"
#include "storage_pool_control_menu.h"
#include "storage_pool_control_thread.h"

class VirtStoragePoolControl : public QMainWindow
{
    Q_OBJECT
public:
    explicit VirtStoragePoolControl(QWidget *parent = 0);
    ~VirtStoragePoolControl();

signals:
    void storagePoolMsg(QString&);

private:
    QString                    currConnName;
    QSettings                  settings;
    StoragePoolModel          *storagePoolModel = NULL;
    QTreeView                 *storagePoolList = NULL;
    StoragePoolToolBar         *toolBar;

    virConnect                *currWorkConnect = NULL;
    StoragePoolControlThread  *stPoolControlThread = NULL;

    int                        interval;
    int                        timerId;

public slots:
    bool getThreadState() const;
    void stopProcessing();
    bool setCurrentWorkConnect(virConnect*);
    void setListHeader(QString&);

private slots:
    void timerEvent(QTimerEvent*);
    void resultReceiver(StoragePoolActions, QStringList);
    void msgRepeater(QString);
    void changeDockVisibility();

    void storagePoolClicked(const QPoint&);
    void storagePoolDoubleClicked(const QModelIndex&);
    void execAction(const QStringList&);
    void newVirtStoragePoolFromXML(const QStringList&);

};

#endif // STORAGE_POOL_CONTROL_H
