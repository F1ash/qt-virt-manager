#ifndef REDIRDEV_DEVICE_H
#define REDIRDEV_DEVICE_H

#include <QListWidget>
#include "create_widgets/domain/_qwidget.h"
#include "address_widgets/usb_addr.h"

class RedirDevDevice : public _QWidget
{
    Q_OBJECT
public:
    explicit RedirDevDevice(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QComboBox       *type;

    QCheckBox       *useAddress;
    USBAddr         *address;

    QLabel          *hostLabel;
    QLabel          *portLabel;
    QLineEdit       *host;
    QSpinBox        *port;
    QHBoxLayout     *sourceLayout;
    QWidget         *source;

    QCheckBox       *filtered;
    QListWidget     *filter;

    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument getDevDocument() const;

private slots:
    void typeChanged(int);
};

#endif // REDIRDEV_DEVICE_H
