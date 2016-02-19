#ifndef QSPICEUSBDEVICEMANAGER_H
#define QSPICEUSBDEVICEMANAGER_H

#include "qspicesession.h"
#include <QStringList>

class QSpiceUsbDeviceManager : public QSpiceObject
{
    Q_OBJECT
public:
    explicit QSpiceUsbDeviceManager(
            QObject        *parent  = nullptr,
            QSpiceSession  *s       = nullptr);

    QStringList spiceUsbDeviceManager_get_devices();
    QStringList spiceUsbDeviceManager_get_devices_with_filter
                                                    (QString&);
    bool        spiceUsbDeviceManager_is_device_connected
                                                    (QString&);
    static void spiceUsbDeviceManager_connect_device_finish
                                                    (void*, void*, void*);

signals:
    void        autoConnectFailed(QString&, QString&);
    void        deviceAdded(QString&);
    void        deviceInfo(QString&, QString&);
    void        deviceRemoved(QString&);

protected:
    friend class QSpiceHelper;
    void        init(QSpiceSession *);

public slots:
    void        spiceUsbDeviceManager_connect_device(QString&);
    void        spiceUsbDeviceManager_disconnect_device(QString&);
};

#endif // QSPICEUSBDEVICEMANAGER_H
