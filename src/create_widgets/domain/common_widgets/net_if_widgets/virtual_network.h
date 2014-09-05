#ifndef VIRTUAL_NETWORK_H
#define VIRTUAL_NETWORK_H

#include "create_widgets/domain/_qwidget.h"
#include "mac_address.h"
#include "virtual_port.h"

class Virtual_Network : public _QWidget
{
    Q_OBJECT
public:
    explicit Virtual_Network(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QLabel          *networkLabel, *targetLabel;
    QComboBox       *network;
    QLineEdit       *target;
    QGridLayout     *baseLayout;
    QWidget         *baseWdg;
    MAC_Address     *mac;
    VirtualPort     *virtPort;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument getDevDocument() const;

private slots:
    void networkChanged(int);
    void setAvailableVirtNetworks();
};

#endif // VIRTUAL_NETWORK_H
