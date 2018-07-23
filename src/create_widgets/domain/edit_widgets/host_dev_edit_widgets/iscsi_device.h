#ifndef ISCSI_DEVICE_H
#define ISCSI_DEVICE_H

#include <QWidget>
#include <QSpinBox>
#include "create_widgets/storage/_create_storage_widgets/_auth.h"

class ISCSI_Device : public QWidget
{
    Q_OBJECT
public:
    explicit ISCSI_Device(
            QWidget        *parent     = Q_NULLPTR,
            virConnectPtr*  connPtrPtr = Q_NULLPTR);
    QLineEdit           *host;
    QSpinBox            *port;
    _Storage_Auth       *auth;

signals:
    void                 dataChanged();

private:
    QLabel              *hostLabel, *portLabel;
    QHBoxLayout         *hostLayout;
    QWidget             *hostWdg;
    QVBoxLayout         *commonLayout;

public slots:
    void                 stateChanged();
};

#endif // ISCSI_DEVICE_H
