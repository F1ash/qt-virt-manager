#ifndef CONSOLE_DEVICE_H
#define CONSOLE_DEVICE_H

#include "char_device.h"

class ConsoleDevice : public CharDevice
{
    Q_OBJECT
public:
    explicit ConsoleDevice(
            QWidget *parent = NULL,
            virConnectPtr conn = NULL,
            virDomainPtr domain = NULL);
    QComboBox       *targetType;

signals:

private:

public slots:
QDomDocument getDataDocument() const;
};

#endif // CONSOLE_DEVICE_H
