#ifndef VIRTUAL_NETWORK_H
#define VIRTUAL_NETWORK_H

#include "create_widgets/domain/_qwidget.h"
#include "nic_model.h"
#include "mac_address.h"
#include "virtual_port.h"
#include "create_widgets/domain/common_widgets/device_address.h"

class Virtual_Network : public _QWidget
{
    Q_OBJECT
public:
    explicit Virtual_Network(
            QWidget        *parent      = NULL,
            virConnectPtr*  connPtrPtr  = NULL);

private:
    QLabel          *networkLabel, *targetLabel;
    QComboBox       *network;
    QLineEdit       *target;
    QGridLayout     *baseLayout;
    QWidget         *baseWdg;
    MAC_Address     *mac;
    NIC_Model       *nicModel;
    VirtualPort     *virtPort;
    DeviceAddress   *addr;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);

private slots:
    void             networkChanged(int);
    void             setAvailableVirtNetworks();
};

#endif // VIRTUAL_NETWORK_H
