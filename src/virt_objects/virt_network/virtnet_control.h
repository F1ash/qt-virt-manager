#ifndef VIRTNET_CONTROL_H
#define VIRTNET_CONTROL_H

#include "virt_objects/virt_entity_control.h"
#include "virtnet_model.h"
#include "virtnet_toolbar.h"
#include "virtnet_control_menu.h"
#include "net_list_delegate.h"
#include "create_widgets/network/create_virt_network_adv.h"

class VirtNetControl : public VirtEntityControl
{
    Q_OBJECT
public:
    explicit VirtNetControl(QWidget *parent = nullptr);
    ~VirtNetControl();

signals:
    void                 networkToEditor(TASK);

private:
    VirtNetModel        *virtNetModel;
    VirtNetToolBar      *toolBar;
    NetworkListDelegate *netListDlg;

public slots:
    void                 stopProcessing();
    bool                 setCurrentWorkConnect(virConnectPtr*);
    void                 setListHeader(QString&);
    void                 resultReceiver(Result);

private slots:
    void                 reloadState();
    void                 changeDockVisibility();
    void                 entityClicked(const QPoint&);
    void                 entityDoubleClicked(const QModelIndex&);
    void                 execAction(const QStringList&);
    void                 newVirtEntityFromXML(const OFILE_TASK&);
};

#endif // VIRTNET_CONTROL_H
