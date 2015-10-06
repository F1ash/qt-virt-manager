
#include "qspicehelper.h"
#include "qspiceusbdevicemanager.h"
#include <usb-device-manager.h>

QSpiceUsbDeviceManager::QSpiceUsbDeviceManager(
        QObject *parent, QSpiceSession *s) :
    QSpiceObject(parent)
{
    gobject = NULL;
    init(s);
}
QSpiceUsbDeviceManager::~QSpiceUsbDeviceManager()
{
    unrefManager();
}

void QSpiceHelper::auto_connect_failed(SpiceUsbDeviceManager *manager,
                                       SpiceUsbDevice        *device,
                                       GError                *error,
                                       gpointer               user_data)
{
    Q_UNUSED(manager)
    QSpiceUsbDeviceManager *_manager =
            static_cast<QSpiceUsbDeviceManager *>(user_data);
    QString err, dev;
    err.append(error->code);
    err.append(error->message);
    gchar *dev_desc = spice_usb_device_get_description(device, NULL);
    dev.append(dev_desc);
    if ( dev_desc ) g_free(dev_desc);
    emit _manager->autoConnectFailed(dev, err);
}

void QSpiceHelper::device_added(SpiceUsbDeviceManager *manager,
                                SpiceUsbDevice        *device,
                                gpointer               user_data)
{
    Q_UNUSED(manager)
    QSpiceUsbDeviceManager *_manager =
            static_cast<QSpiceUsbDeviceManager *>(user_data);
    QString dev;
    gchar *dev_desc = spice_usb_device_get_description(device, NULL);
    dev.append(dev_desc);
    if ( dev_desc ) g_free(dev_desc);
    emit _manager->deviceAdded(dev);
}

void QSpiceHelper::device_error(SpiceUsbDeviceManager *manager,
                                SpiceUsbDevice        *device,
                                GError                *error,
                                gpointer               user_data)
{
    Q_UNUSED(manager)
    QSpiceUsbDeviceManager *_manager =
            static_cast<QSpiceUsbDeviceManager *>(user_data);
    QString err, dev;
    err.append(error->code);
    err.append(error->message);
    gchar *dev_desc = spice_usb_device_get_description(device, NULL);
    dev.append(dev_desc);
    if ( dev_desc ) g_free(dev_desc);
    emit _manager->deviceError(dev, err);
}

void QSpiceHelper::device_removed(SpiceUsbDeviceManager *manager,
                                  SpiceUsbDevice        *device,
                                  gpointer               user_data)
{
    Q_UNUSED(manager)
    QSpiceUsbDeviceManager *_manager =
            static_cast<QSpiceUsbDeviceManager *>(user_data);
    QString dev;
    gchar *dev_desc = spice_usb_device_get_description(device, NULL);
    dev.append(dev_desc);
    if ( dev_desc ) g_free(dev_desc);
    emit _manager->deviceRemoved(dev);
}

void QSpiceUsbDeviceManager::init(QSpiceSession *session)
{
    SpiceSession *_session = static_cast<SpiceSession *>(session->gobject);
    if ( _session ) {
        GError **err = NULL;
        gobject = spice_usb_device_manager_get(_session, err);
        if ( gobject ) {
            g_signal_connect(gobject, "auto-connect-failed",
                             (GCallback) QSpiceHelper::auto_connect_failed, this);
            g_signal_connect(gobject, "device-added",
                             (GCallback) QSpiceHelper::device_added, this);
            g_signal_connect(gobject, "device-error",
                             (GCallback) QSpiceHelper::device_error, this);
            g_signal_connect(gobject, "device-removed",
                             (GCallback) QSpiceHelper::device_removed, this);
            qDebug()<<"UsbDeviceManager initiated";
        } else {
            qDebug()<<"UsbDeviceManager not initiated";
        }
    };
}

void* QSpiceUsbDeviceManager::spice_usb_device_manager_get_devices()
{

}

void* QSpiceUsbDeviceManager::spice_usb_device_manager_get_devices_with_filter
                                                (const char*)
{

}

bool QSpiceUsbDeviceManager::spice_usb_device_manager_is_device_connected
                                                (void*)
{
    return true;
}

void QSpiceUsbDeviceManager::spice_usb_device_manager_disconnect_device
                                                (void*)
{

}

bool QSpiceUsbDeviceManager::spice_usb_device_manager_can_redirect_device
                                                (void*, void*)
{
    return true;
}

void QSpiceUsbDeviceManager::spice_usb_device_manager_connect_device_async
                                                (void*, void*, void*, void*)
{

}

bool QSpiceUsbDeviceManager::spice_usb_device_manager_connect_device_finish
                                                (void*, void*)
{
    return true;
}

char* QSpiceUsbDeviceManager::spice_usb_device_get_description
                                                (void*, const char*)
{
    char *_desc;
    return _desc;
}

/* public slots */
void QSpiceUsbDeviceManager::unrefManager()
{
    if (gobject) gobject = NULL;
}
