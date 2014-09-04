#ifndef NET_INTERFACES_H
#define NET_INTERFACES_H

#include "net_if_widgets/tcp_tunnel.h"
#include "net_if_widgets/multicast_tunnel.h"
#include "net_if_widgets/pci_passthrough.h"
#include "net_if_widgets/direct_attachment.h"

class NetInterfaces : public _QWidget
{
    Q_OBJECT
public:
    explicit NetInterfaces(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QLabel          *typeLabel;
    QComboBox       *type;
    QHBoxLayout     *typeLayout;
    QWidget         *typeWdg;
    _QWidget        *info = NULL;
    QVBoxLayout     *commonLayout;

    QString          connType;

public slots:
    QDomDocument getDevDocument() const;

private slots:
    void typeChanged(int);
};

#endif // NET_INTERFACES_H
