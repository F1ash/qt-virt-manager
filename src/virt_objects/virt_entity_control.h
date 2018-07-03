#ifndef VIRT_ENTITY_CONTROL_H
#define VIRT_ENTITY_CONTROL_H

#include <QMainWindow>
#include "tree_view.h"
#include <QSettings>
#include <QTime>
#include <QUrl>
#include <QDesktopServices>
#include "virt_objects/virt_entity_config.h"
//#include <QDebug>

class VirtEntityControl : public QMainWindow
{
    Q_OBJECT
public:
    explicit VirtEntityControl(QWidget *parent = nullptr);
    virtual ~VirtEntityControl()               = 0;
    QString                  currConnName;
    QSettings                settings;
    TreeView                *entityList;
    virConnectPtr           *ptr_ConnPtr = nullptr;

    virtual void             stopProcessing();
    virtual bool             setCurrentWorkConnection(virConnectPtr*);
    virtual void             setListHeader(const QString&);
    virtual void             reloadState();
    virtual void             changeDockVisibility();
    virtual void             entityClicked(const QPoint&);
    virtual void             entityDoubleClicked(const QModelIndex&);
    virtual void             execAction(const QStringList&);
    virtual void             newVirtEntityFromXML(const QStringList&);

    void                     setUsageInSoftTouched(bool);

signals:
    void                     entityMsg(const QString&);
    void                     addNewTask(TASK*);
    void                     ptrIsNull();
    void                     entityListUpdated();

public slots:
    void                     msgRepeater(const QString&);
    virtual void             resultReceiver(Result);
};

#endif // VIRT_ENTITY_CONTROL_H
