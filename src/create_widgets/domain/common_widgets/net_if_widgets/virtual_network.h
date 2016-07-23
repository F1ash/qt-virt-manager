#ifndef VIRTUAL_NETWORK_H
#define VIRTUAL_NETWORK_H

#include "create_widgets/common/_qwidget_threaded.h"
#include "nic_model.h"
#include "mac_address.h"
#include "virtual_port.h"
#include "nwfilter_parameters.h"
#include "create_widgets/domain/device_address.h"

class virtNet_HlpThread : public qwdHelpThread
{
    Q_OBJECT
public:
    explicit virtNet_HlpThread(
            QObject        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr);
    QStringList      nwFilters;
    void             run();
signals:
    void             result(QStringList&);
};

class Virtual_Network : public _QWidget_Threaded
{
    Q_OBJECT
public:
    explicit Virtual_Network(
            QWidget        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr);

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
    NWFilter_Params *nwFilterParams;
    QVBoxLayout     *commonLayout;

    virtNet_HlpThread
                    *hlpThread;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);

private slots:
    void             networkChanged(int);
    void             setAvailableVirtNetworks(QStringList&);
    void             emitCompleteSignal();
};

#endif // VIRTUAL_NETWORK_H
