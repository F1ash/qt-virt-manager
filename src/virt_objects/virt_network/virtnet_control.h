#ifndef VIRTNET_CONTROL_H
#define VIRTNET_CONTROL_H

#include "virt_objects/virt_entity_control.h"
#include "virtnet_model.h"
#include "virtnet_toolbar.h"
#include "virtnet_control_menu.h"
#include "net_list_delegate.h"

class VirtNetControl : public VirtEntityControl
{
    Q_OBJECT
public:
    explicit VirtNetControl(QWidget *parent = Q_NULLPTR);
    ~VirtNetControl();

signals:
    void                 networkToEditor(TASK*);

private:
    VirtNetModel        *virtNetModel;
    VirtNetToolBar      *toolBar;
    NetworkListDelegate *netListDlg;

public slots:
    void                 stopProcessing();
    bool                 setCurrentWorkConnection(virConnectPtr*);
    void                 setListHeader(const QString&);
    void                 resultReceiver(Result*);
    void                 reloadState();

private slots:
    void                 changeDockVisibility();
    void                 entityClicked(const QPoint&);
    void                 entityDoubleClicked(const QModelIndex&);
    void                 execAction(const Act_Param&);
    void                 newVirtEntityFromXML(const Act_Param&);
};

#endif // VIRTNET_CONTROL_H
