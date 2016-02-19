#ifndef VIRTNET_CONTROL_H
#define VIRTNET_CONTROL_H

#include "virt_objects/virt_entity_control.h"
#include "virtnet_model.h"
#include "virtnet_toolbar.h"
#include "virtnet_control_menu.h"
#include "create_widgets/network/create_virt_network.h"

class VirtNetControl : public VirtEntityControl
{
    Q_OBJECT
public:
    explicit VirtNetControl(QWidget *parent = nullptr);
    ~VirtNetControl();

private:
    VirtNetModel        *virtNetModel;
    VirtNetToolBar      *toolBar;

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
    void                 newVirtEntityFromXML(const QStringList&);
    void                 doneEntityCreationDialog();
};

#endif // VIRTNET_CONTROL_H
