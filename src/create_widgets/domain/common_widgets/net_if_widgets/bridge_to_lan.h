#ifndef BRIDGE_TO_LAN_H
#define BRIDGE_TO_LAN_H

#include "create_widgets/common/_qwidget_threaded.h"
#include "mac_address.h"
#include "virtual_port.h"
#include "nwfilter_parameters.h"
#include "create_widgets/domain/device_address.h"

class bridge_HlpThread : public qwdHelpThread
{
    Q_OBJECT
public:
    explicit bridge_HlpThread(
            QObject        *parent      = Q_NULLPTR,
            virConnectPtr*  connPtrPtr  = Q_NULLPTR);
    QStringList      nwFilters;
    void             run();
signals:
    void             result(QStringList&);
};

class Bridge_to_LAN : public _QWidget_Threaded
{
    Q_OBJECT
public:
    explicit Bridge_to_LAN(
            QWidget        *parent      = Q_NULLPTR,
            virConnectPtr*  connPtrPtr  = Q_NULLPTR);

private:
    QLabel          *bridgeLabel, *targetLabel;
    QLineEdit       *bridge, *target;
    QGridLayout     *baseLayout;
    QWidget         *baseWdg;
    MAC_Address     *mac;
    VirtualPort     *virtPort;
    DeviceAddress   *addr;
    NWFilter_Params *nwFilterParams;
    QVBoxLayout     *commonLayout;

    bridge_HlpThread
                    *hlpThread;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);

private slots:
    void             emitCompleteSignal();
};

#endif // BRIDGE_TO_LAN_H
