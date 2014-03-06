#ifndef VIRTNET_CONTROL_H
#define VIRTNET_CONTROL_H

#include <QMainWindow>
#include <QGridLayout>
#include <QTreeView>
#include <QSettings>
#include <QTime>
#include <QTimerEvent>
#include <QProcess>
#include <QDebug>
#include "virtnet_model.h"
#include "virtnet_toolbar.h"
#include "virtnet_control_menu.h"
#include "net_control_thread.h"

class VirtNetControl : public QMainWindow
{
    Q_OBJECT
public:
    explicit VirtNetControl(QWidget *parent = 0);
    ~VirtNetControl();

signals:
    void netMsg(QString&);

private:
    QString                 currConnName;
    QSettings               settings;
    VirtNetModel           *virtNetModel = NULL;
    QTreeView              *virtNetList = NULL;
    VirtNetToolBar         *toolBar;

    virConnect             *currWorkConnect = NULL;
    NetControlThread       *netControlThread = NULL;

    int                     interval;
    int                     timerId;

public slots:
    bool getThreadState() const;
    void stopProcessing();
    bool setCurrentWorkConnect(virConnect*);
    void setListHeader(QString&);

private slots:
    void timerEvent(QTimerEvent*);
    void resultReceiver(NetActions, QStringList);
    void msgRepeater(QString);
    void changeDockVisibility();

    void networkClicked(const QPoint&);
    void networkDoubleClicked(const QModelIndex&);
    void execAction(const QStringList&);
    void newVirtNetworkFromXML(const QStringList&);

    // TODO: virNetworkUpdate()
    // Update the definition of an existing network,
    // either its live running state, its persistent configuration, or both.
};

#endif // VIRTNET_CONTROL_H
