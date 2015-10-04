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
    void*       spice_usb_device_manager_get_devices();
    void*       spice_usb_device_manager_get_devices_with_filter
                                                    (const char*);
    bool        spice_usb_device_manager_is_device_connected
                                                    (void*);
    void                spice_usb_device_manager_disconnect_device
                                                    (void*);
    bool        spice_usb_device_manager_can_redirect_device
                                                    (void*, void*);
    void        spice_usb_device_manager_connect_device_async
                                                    (void*, void*, void*, void*);
    bool        spice_usb_device_manager_connect_device_finish
                                                    (void*, void*);
    char*       spice_usb_device_get_description    (void*, const char*);

signals:
    void        autoConnectFailed();
    void        deviceAdded();
    void        deviceError();
    void        deviceRemoved();

protected:
    friend class QSpiceHelper;
    void        init(QSpiceSession *);

public slots:
};

#endif // QSPICEUSBDEVICEMANAGER_H
