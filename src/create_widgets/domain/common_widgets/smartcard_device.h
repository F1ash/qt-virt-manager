#ifndef SMARTCARD_DEVICE_H
#define SMARTCARD_DEVICE_H

#include "create_widgets/domain/_qwidget.h"
#include "device_address.h"
#include "channel_device.h"

class SmartCardDevice : public _QWidget
{
    Q_OBJECT
public:
    explicit SmartCardDevice(QWidget *parent = 0);
    DeviceAddress   *addr;
    QComboBox       *mode;
    ChannelDevice   *channel;

signals:

private:
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDevDocument() const;

private slots:
    void             modeTypeChanged(int);
};

#endif // SMARTCARD_DEVICE_H
