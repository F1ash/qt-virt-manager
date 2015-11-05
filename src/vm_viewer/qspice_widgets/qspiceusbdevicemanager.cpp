
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
    Q_UNUSED(manager)
    QSpiceUsbDeviceManager *_manager =
            static_cast<QSpiceUsbDeviceManager *>(user_data);
    if ( NULL==_manager ) return;
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
    if ( NULL==_manager ) return;
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
    if ( NULL==_manager ) return;
    QString err, dev;
    err.append(error->code);
    err.append(error->message);
    gchar *dev_desc = spice_usb_device_get_description(device, NULL);
    dev.append(dev_desc);
    if ( dev_desc ) g_free(dev_desc);
    emit _manager->deviceInfo(dev, err);
}

void QSpiceHelper::device_removed(SpiceUsbDeviceManager *manager,
                                  SpiceUsbDevice        *device,
                                  gpointer               user_data)
{
    Q_UNUSED(manager)
    QSpiceUsbDeviceManager *_manager =
            static_cast<QSpiceUsbDeviceManager *>(user_data);
    if ( NULL==_manager ) return;
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
            //qDebug()<<"UsbDeviceManager initiated";
        } else {
            //qDebug()<<"UsbDeviceManager not initiated";
        }
    };
}

QStringList QSpiceUsbDeviceManager::spiceUsbDeviceManager_get_devices()
{
    QStringList _devList;
    GPtrArray *_devs = spice_usb_device_manager_get_devices(
                (SpiceUsbDeviceManager*)gobject);
    size_t count = _devs->len;
    for ( uint i = 0; i<count; i++ ) {
        SpiceUsbDevice *_dev = (SpiceUsbDevice *)_devs->pdata[i];
        QString dev;
        gchar *dev_desc = spice_usb_device_get_description(_dev, "%s %s<||>%s at %d-%d");
        dev.append(dev_desc);
        if ( dev_desc ) g_free(dev_desc);
        _devList.append(dev);
    };
    return _devList;
}

QStringList QSpiceUsbDeviceManager::spiceUsbDeviceManager_get_devices_with_filter
                                                (QString &filter)
{
    QStringList _devList;
    GPtrArray *_devs = spice_usb_device_manager_get_devices_with_filter(
                (SpiceUsbDeviceManager*)gobject, filter.toUtf8().data());
    size_t count = _devs->len;
    for ( uint i = 0; i<count; i++ ) {
        SpiceUsbDevice *_dev = (SpiceUsbDevice *)_devs->pdata[i];
        QString dev;
        gchar *dev_desc = spice_usb_device_get_description(_dev, "%s %s<||>%s at %d-%d");
        dev.append(dev_desc);
        if ( dev_desc ) g_free(dev_desc);
        _devList.append(dev);
    };
    return _devList;
}

bool QSpiceUsbDeviceManager::spiceUsbDeviceManager_is_device_connected
                                                (QString &_id)
{
    bool _res = false;
    if ( _id.isEmpty() ) return _res;
    GPtrArray *_devs = spice_usb_device_manager_get_devices(
                (SpiceUsbDeviceManager*)gobject);
    size_t count = _devs->len;
    for ( uint i = 0; i<count; i++ ) {
        SpiceUsbDevice *_dev = (SpiceUsbDevice *)_devs->pdata[i];
        QString dev;
        gchar *dev_desc = spice_usb_device_get_description(_dev, "%s %s %s at %d-%d");
        dev.append(dev_desc);
        if ( dev_desc ) g_free(dev_desc);
        if ( dev.contains(_id) ) {
            _res = spice_usb_device_manager_is_device_connected(
                        (SpiceUsbDeviceManager *)gobject, _dev);
            break;
        };
    };
    return _res;
}

void QSpiceUsbDeviceManager::spiceUsbDeviceManager_connect_device_finish
                                                (void *self, void *res, void *err)
{
    GError **errors = (GError **)err;
    GAsyncResult *result = (GAsyncResult *)res;
    bool _res = spice_usb_device_manager_connect_device_finish(
                (SpiceUsbDeviceManager*)self, result, errors);
    if ( !_res ) {
        QSpiceUsbDeviceManager *obj = static_cast<QSpiceUsbDeviceManager*>(
                    g_async_result_get_user_data(result));
        QString err;
        size_t count = sizeof(errors)/sizeof(*errors);
        for ( uint i = 0; i<count; i++ ) {
            GError *error = errors[i];
            err.append(error->code);
            err.append(error->message);
            err.append('\n');
        };
        QString dev("???");
        emit obj->deviceInfo(dev, err);
    };
}


/* public slots */
void QSpiceUsbDeviceManager::spiceUsbDeviceManager_connect_device(QString &_id)
{
    if ( _id.isEmpty() ) return;
    GPtrArray *_devs = spice_usb_device_manager_get_devices(
                (SpiceUsbDeviceManager*)gobject);
    size_t count = _devs->len;
    for ( uint i = 0; i<count; i++ ) {
        SpiceUsbDevice *_dev = (SpiceUsbDevice *)_devs->pdata[i];
        QString dev;
        gchar *dev_desc = spice_usb_device_get_description(_dev, "%s %s %s at %d-%d");
        dev.append(dev_desc);
        if ( dev_desc ) g_free(dev_desc);
        if ( dev.contains(_id) ) {
            GError **errors = NULL;
            bool possibility = spice_usb_device_manager_can_redirect_device(
                        (SpiceUsbDeviceManager *)gobject, _dev, errors);
            if (!possibility) {
                QString err;
                size_t count = sizeof(errors)/sizeof(*errors);
                for ( uint i = 0; i<count; i++ ) {
                    GError *error = errors[i];
                    err.append(error->code);
                    err.append(error->message);
                    err.append('\n');
                };
                emit deviceInfo(dev, err);
                break;
            };
            spice_usb_device_manager_connect_device_async(
                        (SpiceUsbDeviceManager *)gobject,
                        _dev,
                        NULL,
                        (GAsyncReadyCallback)spiceUsbDeviceManager_connect_device_finish,
                        this);
            QString _msg("connected to guest.");
            emit deviceInfo(dev, _msg);
            break;
        };
    };
}
void QSpiceUsbDeviceManager::spiceUsbDeviceManager_disconnect_device(QString &_id)
{
    if ( _id.isEmpty() ) return;
    GPtrArray *_devs = spice_usb_device_manager_get_devices(
                (SpiceUsbDeviceManager*)gobject);
    size_t count = _devs->len;
    for ( uint i = 0; i<count; i++ ) {
        SpiceUsbDevice *_dev = (SpiceUsbDevice *)_devs->pdata[i];
        QString dev;
        gchar *dev_desc = spice_usb_device_get_description(_dev, "%s %s %s at %d-%d");
        dev.append(dev_desc);
        if ( dev_desc ) g_free(dev_desc);
        if ( dev.contains(_id) ) {
            spice_usb_device_manager_disconnect_device(
                        (SpiceUsbDeviceManager *)gobject, _dev);
            QString _msg("disconnected from guest.");
            emit deviceInfo(dev, _msg);
            break;
        };
    };
}
