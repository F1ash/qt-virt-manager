
#include "qspice-helper.h"
#include "qspice-usbdevice-manager.h"

struct RES {
    void        *obj    = nullptr;
    QString      id;
    bool         result = false;
    int          idx    = -1;
    int          size   = 0;
};

QSpiceUsbDeviceManager::QSpiceUsbDeviceManager(
        QObject *parent, QSpiceSession *s) :
    QSpiceObject(parent)
{
    gobject = nullptr;
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
    if ( nullptr==_manager ) return;
    QString err, dev;
    if (error!=nullptr) {
        err.append(error->code);
        err.append(error->message);
    };
    gchar *dev_desc = spice_usb_device_get_description(device, nullptr);
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
    if ( nullptr==_manager ) return;
    QString dev;
    gchar *dev_desc = spice_usb_device_get_description(device, nullptr);
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
    if ( nullptr==_manager ) return;
    QString err, dev;
    if (error!=nullptr) {
        err.append(error->code);
        err.append(error->message);
    };
    gchar *dev_desc = spice_usb_device_get_description(device, nullptr);
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
    if ( nullptr==_manager ) return;
    QString dev;
    gchar *dev_desc = spice_usb_device_get_description(device, nullptr);
    dev.append(dev_desc);
    if ( dev_desc ) g_free(dev_desc);
    emit _manager->deviceRemoved(dev);
}

void QSpiceUsbDeviceManager::init(QSpiceSession *session)
{
    SpiceSession *_session = static_cast<SpiceSession *>(session->gobject);
    if ( _session ) {
        GError **err = nullptr;
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

void get_descriptions(gpointer item, void *res)
{
    QStringList *_devList =
            static_cast<QStringList*>(res);
    SpiceUsbDevice *_dev = (SpiceUsbDevice*) item;
    QString dev;
    gchar *dev_desc = spice_usb_device_get_description(
                _dev, "%s %s<||>%s at %d-%d");
    dev.append(dev_desc);
    if ( dev_desc ) g_free(dev_desc);
    if ( _devList!=nullptr ) _devList->append(dev);
}

QStringList QSpiceUsbDeviceManager::spiceUsbDeviceManager_get_devices()
{
    QStringList _devList;
    GPtrArray *_devs = spice_usb_device_manager_get_devices(
                (SpiceUsbDeviceManager*) gobject);
    g_ptr_array_foreach(_devs, get_descriptions, &_devList);
    g_ptr_array_free(_devs, true);
    return _devList;
}

QStringList QSpiceUsbDeviceManager::spiceUsbDeviceManager_get_devices_with_filter
                                                (QString &filter)
{
    QStringList _devList;
    GPtrArray *_devs = spice_usb_device_manager_get_devices_with_filter(
                (SpiceUsbDeviceManager*) gobject, filter.toUtf8().data());
    g_ptr_array_foreach(_devs, get_descriptions, &_devList);
    g_ptr_array_free(_devs, true);
    return _devList;
}

void get_operation_result(gpointer item, void *res)
{
    RES *_res = static_cast<RES*>(res);
    SpiceUsbDevice *_dev = (SpiceUsbDevice*) item;
    QString dev;
    gchar *dev_desc = spice_usb_device_get_description(
                _dev, "%s %s %s at %d-%d");
    dev.append(dev_desc);
    if ( dev_desc ) g_free(dev_desc);
    if ( _res!=nullptr ) {
        if ( dev.contains(_res->id) ) {
            _res->result = spice_usb_device_manager_is_device_connected(
                        (SpiceUsbDeviceManager*) _res->obj, _dev);
        };
    };
}

bool QSpiceUsbDeviceManager::spiceUsbDeviceManager_is_device_connected
                                                (QString &_id)
{
    if ( _id.isEmpty() ) return false;
    RES _res;
    _res.id = _id;
    _res.obj = gobject;
    GPtrArray *_devs = spice_usb_device_manager_get_devices(
                (SpiceUsbDeviceManager*)gobject);
    g_ptr_array_foreach(_devs, get_operation_result, &_res);
    g_ptr_array_free(_devs, true);
    return _res.result;
}

void get_device_idx(gpointer item, void *res)
{
    RES *_res = static_cast<RES*>(res);
    SpiceUsbDevice *_dev = (SpiceUsbDevice*) item;
    QString dev;
    gchar *dev_desc = spice_usb_device_get_description(
                _dev, "%s %s %s at %d-%d");
    dev.append(dev_desc);
    if ( dev_desc ) g_free(dev_desc);
    if ( _res!=nullptr ) {
        if ( dev.contains(_res->id) ) {
            _res->idx = _res->size;
        };
        ++_res->size;
    };
}

void QSpiceUsbDeviceManager::spiceUsbDeviceManager_connect_finish_callback
                                                (void *self, void *res, void *err)
{
    GError **errors = (GError**) err;
    GAsyncResult *result = (GAsyncResult*) res;
    bool _res = spice_usb_device_manager_connect_device_finish(
                (SpiceUsbDeviceManager*) self, result, errors);
    if ( !_res ) {
        QSpiceUsbDeviceManager *obj =
                static_cast<QSpiceUsbDeviceManager*>(
                    g_async_result_get_user_data(result));
        if ( obj==nullptr ) return;
        QString err;
        size_t count = sizeof(errors)/sizeof(*errors);
        for ( uint i = 0; i<count; i++ ) {
            if ( nullptr==errors[i] ) continue;
            GError *error = errors[i];
            err.append(error->code);
            err.append(error->message);
            err.append('\n');
            g_error_free(error);
        };
        QString dev("???");
        emit obj->deviceInfo(dev, err);
    };
}

#if SPICE_GTK_MAJOR_VERSION==0 && SPICE_GTK_MINOR_VERSION>=32
void QSpiceUsbDeviceManager::spiceUsbDeviceManager_disconnect_finish_callback
                                                (void *self, void *res, void *_err)
{
    GError **errors = (GError**) _err;
    GAsyncResult *result = (GAsyncResult*) res;
    bool _res = spice_usb_device_manager_disconnect_device_finish(
                (SpiceUsbDeviceManager*) self, result, errors);
    if ( !_res ) {
        QSpiceUsbDeviceManager *obj =
                static_cast<QSpiceUsbDeviceManager*>(
                    g_async_result_get_user_data(result));
        if ( obj==nullptr ) return;
        QString err;
        if ( nullptr!=errors ) {
            size_t count = sizeof(errors)/sizeof(*errors);
            for ( uint i = 0; i<count; i++ ) {
                if ( nullptr==errors[i] ) continue;
                GError *error = errors[i];
                err.append(error->code);
                err.append(" ");
                err.append(error->message);
                err.append('\n');
                g_error_free(error);
            };
        } else {

        };
        QString dev("???");
        emit obj->deviceInfo(dev, err);
    };
}
#endif

/* public slots */
void QSpiceUsbDeviceManager::spiceUsbDeviceManager_connect_device(QString &_id)
{
    if ( _id.isEmpty() ) return;
    RES _res;
    _res.id = _id;
    GPtrArray *_devs = spice_usb_device_manager_get_devices(
                (SpiceUsbDeviceManager*) gobject);
    g_ptr_array_foreach(_devs, get_device_idx, &_res);
    if ( _res.idx!=-1 ) {
        SpiceUsbDevice *_dev =
                (SpiceUsbDevice*) g_ptr_array_index (_devs, _res.idx);
        GError **errors = nullptr;
        bool possibility = spice_usb_device_manager_can_redirect_device(
                    (SpiceUsbDeviceManager*) gobject, _dev, errors);
        if (!possibility) {
            QString err;
            if ( nullptr!=errors ) {
                size_t count = sizeof(errors)/sizeof(*errors);
                for ( uint i = 0; i<count; i++ ) {
                    if ( nullptr==errors[i] ) continue;
                    GError *error = errors[i];
                    err.append(error->code);
                    err.append(" ");
                    err.append(error->message);
                    err.append('\n');
                    g_error_free(error);
                };
            } else {
                err.append(tr("connection impossible."));
            };
            emit deviceInfo(_id, err);
        } else {
            bool connected = spice_usb_device_manager_is_device_connected(
                        (SpiceUsbDeviceManager*) gobject, _dev);
            QString _msg;
            if ( !connected ) {
                spice_usb_device_manager_connect_device_async(
                            (SpiceUsbDeviceManager*) gobject,
                            _dev,
                            nullptr,
                            (GAsyncReadyCallback)spiceUsbDeviceManager_connect_finish_callback,
                            this);
                _msg.append(tr("connected to guest."));
            } else {
                _msg.append(tr("connected to guest already."));
            };
            emit deviceInfo(_id, _msg);
        };
    };
    g_ptr_array_free(_devs, true);
}
void QSpiceUsbDeviceManager::spiceUsbDeviceManager_disconnect_device(QString &_id)
{
    if ( _id.isEmpty() ) return;
    RES _res;
    _res.id = _id;
    GPtrArray *_devs = spice_usb_device_manager_get_devices(
                (SpiceUsbDeviceManager*) gobject);
    g_ptr_array_foreach(_devs, get_device_idx, &_res);
    if ( _res.idx!=-1 ) {
        SpiceUsbDevice *_dev =
                (SpiceUsbDevice*) g_ptr_array_index (_devs, _res.idx);
        bool connected = spice_usb_device_manager_is_device_connected(
                    (SpiceUsbDeviceManager*) gobject, _dev);
        QString _msg;
        if ( connected ) {
#if SPICE_GTK_MAJOR_VERSION==0 && SPICE_GTK_MINOR_VERSION>=32
            spice_usb_device_manager_disconnect_device_async(
                        (SpiceUsbDeviceManager*) gobject,
                        _dev,
                        nullptr,
                        (GAsyncReadyCallback)spiceUsbDeviceManager_disconnect_finish_callback,
                        this);
#else
            spice_usb_device_manager_disconnect_device(
                        (SpiceUsbDeviceManager*) gobject, _dev);
#endif
            _msg.append(tr("disconnected from guest."));
        } else {
            _msg.append(tr("disconnected from guest already."));
        };
        emit deviceInfo(_id, _msg);
    };
    g_ptr_array_free(_devs, true);
}
