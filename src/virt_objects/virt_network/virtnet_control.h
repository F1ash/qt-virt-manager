#ifndef VIRTNET_CONTROL_H
#define VIRTNET_CONTROL_H

#include <QMainWindow>
#include <QGridLayout>
#include <QTreeView>
#include <QSettings>
#include <QTime>
#include <QUrl>
#include <QDesktopServices>
#include <QMessageBox>
#include <QDebug>
#include "virtnet_model.h"
#include "virtnet_toolbar.h"
#include "virtnet_control_menu.h"
#include "net_control_thread.h"
#include "create_widgets/network/create_virt_network.h"

class VirtNetControl : public QMainWindow
{
    Q_OBJECT
public:
    explicit VirtNetControl(QWidget *parent = NULL);
    ~VirtNetControl();

signals:
    void                 netMsg(QString&);
    void                 addNewTask(virConnectPtr, QStringList&);

private:
    QString              currConnName;
    QSettings            settings;
    VirtNetModel        *virtNetModel = NULL;
    QTreeView           *virtNetList = NULL;
    VirtNetToolBar      *toolBar;

    virConnect          *currWorkConnect = NULL;
    NetControlThread    *netControlThread = NULL;

public slots:
    bool                 getThreadState() const;
    void                 stopProcessing();
    bool                 setCurrentWorkConnect(virConnect*);
    void                 setListHeader(QString&);

private slots:
    void                 resultReceiver(Result);
    void                 msgRepeater(QString);
    void                 changeDockVisibility();

    void                 networkClicked(const QPoint&);
    void                 networkDoubleClicked(const QModelIndex&);
    void                 execAction(const QStringList&);
    void                 newVirtNetworkFromXML(const QStringList&);
};

#endif // VIRTNET_CONTROL_H
