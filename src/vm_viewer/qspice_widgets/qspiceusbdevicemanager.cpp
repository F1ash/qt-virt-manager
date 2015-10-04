
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


void QSpiceHelper::auto_connect_failed(SpiceUsbDeviceManager *manager,
                                       SpiceUsbDevice        *device,
                                       GError                *error,
                                       gpointer               user_data)
{

}

void QSpiceHelper::device_added(SpiceUsbDeviceManager *manager,
                                SpiceUsbDevice        *device,
                                gpointer               user_data)
{

}

void QSpiceHelper::device_error(SpiceUsbDeviceManager *manager,
                                SpiceUsbDevice        *device,
                                GError                *error,
                                gpointer               user_data)
{

}

void QSpiceHelper::device_removed(SpiceUsbDeviceManager *manager,
                                  SpiceUsbDevice        *device,
                                  gpointer               user_data)
{

}


void QSpiceUsbDeviceManager::init(QSpiceSession *session)
{
    GError **err = NULL;
    SpiceSession *_session = static_cast<SpiceSession *>(session->gobject);
    if ( _session ) {
        gobject = spice_usb_device_manager_get(_session, err);
        g_signal_connect(gobject, "auto-connect-failed",
                         (GCallback) QSpiceHelper::auto_connect_failed, this);
        g_signal_connect(gobject, "device-added",
                         (GCallback) QSpiceHelper::device_added, this);
        g_signal_connect(gobject, "device-error",
                         (GCallback) QSpiceHelper::device_error, this);
        g_signal_connect(gobject, "device-removed",
                         (GCallback) QSpiceHelper::device_removed, this);
        qDebug()<<"usb device manager initiated";
    };
}
