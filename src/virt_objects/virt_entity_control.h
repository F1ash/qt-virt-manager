#ifndef VIRT_ENTITY_CONTROL_H
#define VIRT_ENTITY_CONTROL_H

#include <QMainWindow>
#include <QTreeView>
#include <QSettings>
#include <QTime>
#include <QUrl>
#include <QDesktopServices>
#include <QDebug>
#include "virt_objects/virt_entity_config.h"

class VirtEntityControl : public QMainWindow
{
    Q_OBJECT
public:
    explicit VirtEntityControl(QWidget *parent = NULL);
    virtual ~VirtEntityControl()               = 0;
    QString                  currConnName;
    QSettings                settings;
    QTreeView               *entityList;
    virConnectPtr           *ptr_ConnPtr = NULL;

    virtual void             reloadState();
    virtual void             changeDockVisibility();
    virtual void             entityClicked(const QPoint&);
    virtual void             entityDoubleClicked(const QModelIndex&);
    virtual void             execAction(const QStringList&);
    virtual void             newVirtEntityFromXML(const QStringList&);
    virtual void             doneEntityCreationDialog();

signals:
    void                     entityMsg(QString&);
    void                     addNewTask(TASK);
    void                     ptrIsNull();

public slots:
    void                     msgRepeater(QString&);
    virtual void             stopProcessing();
    virtual bool             setCurrentWorkConnect(virConnectPtr*);
    virtual void             setListHeader(QString&);
    virtual void             resultReceiver(Result);
};

#endif // VIRT_ENTITY_CONTROL_H
