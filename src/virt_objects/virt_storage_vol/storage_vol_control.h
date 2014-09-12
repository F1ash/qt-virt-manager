#ifndef STORAGE_VOL_CONTROL_H
#define STORAGE_VOL_CONTROL_H

#include <QMainWindow>
#include <QGridLayout>
#include <QTreeView>
#include <QSettings>
#include <QTime>
#include <QUrl>
#include <QDesktopServices>
#include <QMessageBox>
#include <QDebug>
#include "storage_vol_model.h"
#include "storage_vol_toolbar.h"
#include "storage_vol_control_menu.h"
#include "storage_vol_control_thread.h"
#include "resize_dialog.h"
#include "create_widgets/domain/common_widgets/disk_widgets/create_volume.h"

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

public slots:
    bool getThreadState() const;
    void stopProcessing();
    bool setCurrentStoragePool(virConnect*, QString&, QString&);
    QString getCurrentVolumeName() const;

private slots:
    void resultReceiver(StorageVolActions, QStringList);
    void msgRepeater(QString);
    void changeDockVisibility();

    void storageVolClicked(const QPoint&);
    void storageVolDoubleClicked(const QModelIndex&);
    void execAction(const QStringList&);
    void newVirtStorageVolFromXML(const QStringList&);

};

#endif // STORAGE_VOL_CONTROL_H
