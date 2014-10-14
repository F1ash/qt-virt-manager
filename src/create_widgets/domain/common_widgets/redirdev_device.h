#ifndef REDIRDEV_DEVICE_H
#define REDIRDEV_DEVICE_H

#include "create_widgets/domain/_qwidget.h"
#include "address_widgets/usb_addr.h"
#include "redir_filter.h"

class RedirDevDevice : public _QWidget
{
    Q_OBJECT
public:
    explicit RedirDevDevice(
            QWidget *parent = NULL,
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
    RedirFilter     *filter;

    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument getDataDocument() const;

private slots:
    void typeChanged(int);
};

#endif // REDIRDEV_DEVICE_H
