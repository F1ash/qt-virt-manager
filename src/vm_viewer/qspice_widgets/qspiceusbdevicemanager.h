#ifndef QSPICEUSBDEVICEMANAGER_H
#define QSPICEUSBDEVICEMANAGER_H

#include "qspicesession.h"

class QSpiceUsbDeviceManager : public QSpiceObject
{
    Q_OBJECT
public:
    explicit QSpiceUsbDeviceManager(
            QObject        *parent  = 0,
            QSpiceSession  *s       = 0);

signals:
    void        autoConnectFailed();
    void        deviceAdded();
    void        deviceError();
    void        deviceRemoved();

protected:
    void        init(QSpiceSession *);

public slots:
};

#endif // QSPICEUSBDEVICEMANAGER_H
