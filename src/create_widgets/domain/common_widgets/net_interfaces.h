#ifndef NET_INTERFACES_H
#define NET_INTERFACES_H

#include <QStackedWidget>
#include "net_if_widgets/tcp_tunnel.h"
#include "net_if_widgets/multicast_tunnel.h"
#include "net_if_widgets/pci_passthrough.h"
#include "net_if_widgets/direct_attachment.h"
#include "net_if_widgets/generic_ethernet.h"
#include "net_if_widgets/userspace_slirp.h"
#include "net_if_widgets/bridge_to_lan.h"
#include "net_if_widgets/virtual_network.h"
#include "create_widgets/common/_qwidget_threaded.h"

class NetInterfaces : public _QWidget_Threaded
{
    Q_OBJECT
public:
    explicit NetInterfaces(
            QWidget        *parent      = Q_NULLPTR,
            virConnectPtr*  connPtrPtr  = Q_NULLPTR);
    QComboBox       *type;
    QStackedWidget  *info;

private:
    QLabel          *typeLabel;
    QHBoxLayout     *typeLayout;
    QWidget         *typeWdg;
    QVBoxLayout     *commonLayout;

    QString          connType;

public slots:
    QDomDocument     getDataDocument() const;

private slots:
    void             init_wdg();
    void             setWidgets(QString);
};

#endif // NET_INTERFACES_H
