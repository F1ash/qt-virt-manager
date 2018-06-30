#ifndef VIRT_SECRET_CONTROL_H
#define VIRT_SECRET_CONTROL_H

#include "virt_objects/virt_entity_control.h"
#include "secret_model.h"
#include "secret_toolbar.h"
#include "secret_control_menu.h"
#include "secret_list_delegate.h"
#include "create_widgets/secret/create_virt_secret.h"

class VirtSecretControl : public VirtEntityControl
{
    Q_OBJECT
public:
    explicit VirtSecretControl(QWidget *parent = nullptr);
    ~VirtSecretControl();

private:
    VirtSecretModel     *virtSecretModel;
    VirtSecretToolBar   *toolBar;
    SecretListDelegate  *secListDlg;

public slots:
    void                 stopProcessing();
    bool                 setCurrentWorkConnection(virConnectPtr*);
    void                 setListHeader(const QString&);
    QString              getCurrentSecUsage() const;
    QString              getCurrentSecUUID() const;
    QString              getCurrentSecType() const;
    void                 resultReceiver(Result*);
    void                 reloadState();

private slots:
    void                 changeDockVisibility();
    void                 entityClicked(const QPoint&);
    void                 entityDoubleClicked(const QModelIndex&);
    void                 execAction(const Act_Param&);
};

#endif // VIRT_SECRET_CONTROL_H
