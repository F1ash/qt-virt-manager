#ifndef DOMAIN_CONTROL_H
#define DOMAIN_CONTROL_H

#include <QMainWindow>
#include <QGridLayout>
#include <QTreeView>
#include <QSettings>
#include <QTime>
#include <QTimerEvent>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include "domain_model.h"
#include "domain_toolbar.h"
#include "domain_control_menu.h"
#include "domain_control_thread.h"

class VirtDomainControl : public QMainWindow
{
    Q_OBJECT
public:
    explicit VirtDomainControl(QWidget *parent = 0);
    ~VirtDomainControl();

signals:
    void domMsg(QString&);

private:
    QString                  currConnName;
    QSettings                settings;
    DomainModel             *domainModel = NULL;
    QTreeView               *domainList = NULL;
    DomainToolBar           *toolBar;

    virConnect              *currWorkConnect = NULL;
    DomControlThread        *domControlThread = NULL;

    int                      interval;
    int                      timerId;

public slots:
    bool                     getThreadState() const;
    void                     stopProcessing();
    bool                     setCurrentWorkConnect(virConnect*);
    void                     setListHeader(QString&);
    virConnect*              getConnect() const;

private slots:
    void                     timerEvent(QTimerEvent*);
    void                     resultReceiver(DomActions, QStringList);
    void                     msgRepeater(QString);
    void                     changeDockVisibility();

    void                     domainClicked(const QPoint&);
    void                     domainDoubleClicked(const QModelIndex&);
    void                     execAction(const QStringList&);
    void                     newVirtDomainFromXML(const QStringList&);

};

#endif // DOMAIN_CONTROL_H
