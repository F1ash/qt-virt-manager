#ifndef DIRECTAT_TACHMENT_H
#define DIRECTAT_TACHMENT_H

#include "create_widgets/domain/_qwidget.h"
#include "virtual_port.h"
#include "nic_model.h"
#include "mac_address.h"

class DirectAttachment : public _QWidget
{
    Q_OBJECT
public:
    explicit DirectAttachment(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QLabel              *netSourceLabel, *sourceModeLabel;
    QComboBox           *netSource, *sourceMode;
    QWidget             *baseWdg;
    QGridLayout         *baseLayout;
    MAC_Address         *mac;
    NIC_Model           *nicModel;
    VirtualPort         *virtPort;
    QVBoxLayout         *commonLayout;

public slots:
    QDomDocument getDevDocument() const;

private slots:
    void setAvailableSources();
};

#endif // DIRECTAT_TACHMENT_H
