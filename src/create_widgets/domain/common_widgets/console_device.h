#ifndef CONSOLE_DEVICE_H
#define CONSOLE_DEVICE_H

#include "char_device.h"

class ConsoleDevice : public CharDevice
{
    Q_OBJECT
public:
    explicit ConsoleDevice(
            QWidget *parent = 0,
            virConnectPtr conn = NULL,
            virDomainPtr domain = NULL
            );

signals:

private:
    QComboBox       *targetType;

public slots:
QDomDocument getDevDocument() const;
};

#endif // CONSOLE_DEVICE_H
