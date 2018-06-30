#ifndef VIRT_NWFILTER_CONTROL_H
#define VIRT_NWFILTER_CONTROL_H

#include "virt_objects/virt_entity_control.h"
#include "nwfilter_model.h"
#include "nwfilter_toolbar.h"
#include "nwfilter_control_menu.h"
#include "nwfilter_list_delegate.h"
#include "create_widgets/secret/create_virt_secret.h"

class VirtNWFilterControl : public VirtEntityControl
{
    Q_OBJECT
public:
    explicit VirtNWFilterControl(QWidget *parent = nullptr);
    ~VirtNWFilterControl();

signals:
    void                 nwfilterToEditor(TASK*);

private:
    VirtNWFilterModel     *virtNWFilterModel;
    VirtNWFilterToolBar   *toolBar;
    NWFilterListDelegate  *nwfilterListDlg;

public slots:
    void                 stopProcessing();
    bool                 setCurrentWorkConnection(virConnectPtr*);
    void                 setListHeader(const QString&);
    QString              getCurrentNFName() const;
    QString              getCurrentNFUUID() const;
    void                 resultReceiver(Result*);

private slots:
    void                 reloadState();
    void                 changeDockVisibility();
    void                 entityClicked(const QPoint&);
    void                 entityDoubleClicked(const QModelIndex&);
    void                 execAction(const Act_Param&);
    void                 newVirtEntityFromXML(const Act_Param&);
};

#endif // VIRT_NWFILTER_CONTROL_H
