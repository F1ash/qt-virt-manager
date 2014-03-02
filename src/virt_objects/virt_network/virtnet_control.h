#ifndef VIRTNET_CONTROL_H
#define VIRTNET_CONTROL_H

#include <QMainWindow>
#include <QGridLayout>
#include <QTreeView>
#include <QSettings>
#include <QTime>
#include <QTimerEvent>
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

private slots:
    void timerEvent(QTimerEvent*);
    void resultReceiver(QStringList);
    void warningShow(QString, QString);

    // TODO: display XML format of network item

    void networkClicked(const QPoint&);
    void networkDoubleClicked(const QModelIndex&);
    void appendNetworkName(const QStringList&);
};

#endif // VIRTNET_CONTROL_H
