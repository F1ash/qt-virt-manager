#ifndef VIRT_INTERFACE_CONTROL_H
#define VIRT_INTERFACE_CONTROL_H

#include "virt_objects/virt_entity_control.h"
#include "virt_interface_toolbar.h"
#include "virt_interface_model.h"
#include "virt_interface_control_menu.h"
#include "create_widgets/interface/create_interface.h"

class VirtInterfaceControl : public VirtEntityControl
{
    Q_OBJECT
public:
    explicit VirtInterfaceControl(QWidget *parent = NULL);
    ~VirtInterfaceControl();

private:
    VirtIfaceModel      *virtIfaceModel = NULL;
    InterfaceToolBar    *toolBar;

public slots:
    bool                 getThreadState() const;
    void                 stopProcessing();
    bool                 setCurrentWorkConnect(virConnect*);
    void                 setListHeader(QString&);
    void                 resultReceiver(Result);

private slots:
    void                 reloadState();
    void                 changeDockVisibility();
    void                 entityClicked(const QPoint&);
    void                 entityDoubleClicked(const QModelIndex&);
    void                 execAction(const QStringList&);
    void                 newVirtEntityFromXML(const QStringList&);
};

#endif // VIRT_INTERFACE_CONTROL_H
