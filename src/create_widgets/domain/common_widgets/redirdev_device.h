#ifndef REDIRDEV_DEVICE_H
#define REDIRDEV_DEVICE_H

#include "create_widgets/domain/_qwidget.h"
#include "device_address.h"
#include "redir_filter.h"

class RedirDevDevice : public _QWidget
{
    Q_OBJECT
public:
    explicit RedirDevDevice(
            QWidget        *parent  = NULL,
            virConnectPtr*  connPtrPtr = NULL);
    QComboBox       *type;
    QLineEdit       *host;
    QSpinBox        *port;
    DeviceAddress   *address;
    RedirFilter     *filter;

private:
    QLabel          *hostLabel;
    QLabel          *portLabel;
    QHBoxLayout     *sourceLayout;
    QWidget         *source;

    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;

private slots:
    void             typeChanged(int);
};

#endif // REDIRDEV_DEVICE_H
