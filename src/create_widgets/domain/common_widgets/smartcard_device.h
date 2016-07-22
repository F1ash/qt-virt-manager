#ifndef SMARTCARD_DEVICE_H
#define SMARTCARD_DEVICE_H

#include "create_widgets/common/_qwidget.h"
#include "device_address.h"
#include "channel_device.h"

class SmartCardDevice : public _QWidget
{
    Q_OBJECT
public:
    explicit SmartCardDevice(QWidget *parent = nullptr);
    DeviceAddress   *addr;
    QComboBox       *mode;
    ChannelDevice   *channel;

signals:

private:
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;

private slots:
    void             modeTypeChanged(int);
};

#endif // SMARTCARD_DEVICE_H
