#ifndef STORAGE_VOL_CONTROL_H
#define STORAGE_VOL_CONTROL_H

#include <QMainWindow>
#include <QGridLayout>
#include <QTreeView>
#include <QSettings>
#include <QTime>
#include <QTimerEvent>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include "storage_vol_model.h"
#include "storage_vol_toolbar.h"
#include "storage_vol_control_menu.h"
#include "storage_vol_control_thread.h"

class VirtStorageVolControl : public QMainWindow
{
    Q_OBJECT
public:
    explicit VirtStorageVolControl(QWidget *parent = 0);
    ~VirtStorageVolControl();

signals:
    void storageVolMsg(QString&);

private:
    QString                    currConnName;
    QString                    currPoolName;
    QSettings                  settings;
    StorageVolModel           *storageVolModel = NULL;
    QTreeView                 *storageVolList = NULL;
    StorageVolToolBar         *toolBar;

    virConnect                *currWorkConnect = NULL;
    StorageVolControlThread   *stVolControlThread = NULL;

    int                        interval;
    int                        timerId;

public slots:
    bool getThreadState() const;
    void stopProcessing();
    bool setCurrentStoragePool(virConnect*, QString&, QString&);

private slots:
    void timerEvent(QTimerEvent*);
    void resultReceiver(StorageVolActions, QStringList);
    void msgRepeater(QString);
    void changeDockVisibility();

    void storageVolClicked(const QPoint&);
    void storageVolDoubleClicked(const QModelIndex&);
    void execAction(const QStringList&);
    void newVirtStorageVolFromXML(const QStringList&);

};

#endif // STORAGE_VOL_CONTROL_H
