#ifndef VIRT_INTERFACE_CONTROL_H
#define VIRT_INTERFACE_CONTROL_H

#include "virt_objects/virt_entity_control.h"
#include "virt_interface_toolbar.h"
#include "virt_interface_model.h"
#include "virt_interface_control_menu.h"
#include "interface_list_delegate.h"
#include "create_widgets/interface/create_interface.h"

class VirtInterfaceControl : public VirtEntityControl
{
    Q_OBJECT
public:
    explicit VirtInterfaceControl(QWidget *parent = nullptr);
    ~VirtInterfaceControl();

signals:
    void                 ifaceToEditor(TASK*);

private:
    VirtIfaceModel      *virtIfaceModel;
    InterfaceToolBar    *toolBar;
    IfaceListDelegate   *ifaceListDelegate;

public slots:
    void                 stopProcessing();
    bool                 setCurrentWorkConnection(virConnectPtr*);
    void                 setListHeader(const QString&);
    void                 resultReceiver(Result*);

private slots:
    void                 reloadState();
    void                 changeDockVisibility();
    void                 entityClicked(const QPoint&);
    void                 entityDoubleClicked(const QModelIndex&);
    void                 execAction(const Act_Param&);
    void                 newVirtEntityFromXML(const Act_Param&);
};

#endif // VIRT_INTERFACE_CONTROL_H
