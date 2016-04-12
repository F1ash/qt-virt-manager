#ifndef CONSOLE_DEVICE_H
#define CONSOLE_DEVICE_H

#include "char_device.h"

class ConsoleDevice : public CharDevice
{
    Q_OBJECT
public:
    explicit ConsoleDevice(
            QWidget        *parent     = nullptr,
            virConnectPtr*  connPtrPtr = nullptr,
            virDomainPtr    domain     = nullptr);
    QComboBox       *targetType;

signals:

private:

public slots:
    QDomDocument         getDataDocument() const;

private slots:
    void                 init_wdg();
};

#endif // CONSOLE_DEVICE_H
