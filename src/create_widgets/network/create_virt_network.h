#ifndef CREATE_VIRT_NETWORK_H
#define CREATE_VIRT_NETWORK_H

#include "create_widgets/domain/_qwidget.h"
#include "libvirt/libvirt.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>

#define FORWARD_TYPE QStringList()<<"network"<<"bridge"<<"hostdev"<<"mcast"<<"server"<<"user"<<"direct"<<"ethernet"

class CreateVirtNetwork : public _QWidget
{
    Q_OBJECT
public:
    explicit CreateVirtNetwork(QWidget *parent = 0);
    ~CreateVirtNetwork();

signals:

private:
    QWidget         *bridge;
    QGridLayout     *bridgeLayout;
    QLineEdit       *bridgeName;
    QCheckBox       *stp;
    QLabel          *delayLabel;
    QSpinBox        *delay;

    QVBoxLayout     *netDescLayout;
    QLineEdit       *networkName;
    QLabel          *uuid;
    QLineEdit       *source;
    QLineEdit       *target;
    QLineEdit       *address;
    QLineEdit       *mac;

public slots:

};

#endif // CREATE_VIRT_NETWORK_H
