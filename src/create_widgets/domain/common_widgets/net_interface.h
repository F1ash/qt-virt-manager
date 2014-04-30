#ifndef NET_INTERFACE_H
#define NET_INTERFACE_H

#include "create_widgets/domain/_qwidget.h"
#include "create_widgets/network/create_virt_network.h"
#include <QScrollArea>

class NetInterface : public _QWidget
{
    Q_OBJECT
public:
    explicit NetInterface(QWidget *parent = 0, virNetworkPtr *nets = NULL);
    ~NetInterface();

signals:

private:
    virNetworkPtr           *existNetwork;
    QCheckBox               *useExistNetwork;
    QComboBox               *networks;
    QScrollArea             *scroll;
    CreateVirtNetwork       *netDescWdg;

    QVBoxLayout     *commonLayout;

public slots:

private slots:
    void changeUsedNetwork(bool);
    void changeUsedNetwork(QString);

};

#endif // NET_INTERFACE_H
