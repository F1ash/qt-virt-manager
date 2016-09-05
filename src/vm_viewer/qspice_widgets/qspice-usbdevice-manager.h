#ifndef QSPICE_USBDEVICEMANAGER_H
#define QSPICE_USBDEVICEMANAGER_H

#if !defined(QSPICE_WIDGET_H_INSIDE) && !defined(QSPICE_WIDGET_COMPILATION)
#warning "Only <qspice-widget.h> can be included directly"
#endif

#include "qspice-session.h"

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
    static void spiceUsbDeviceManager_connect_finish_callback
                                                    (void*, void*, void*);
#if SPICE_GTK_MAJOR_VERSION==0 && SPICE_GTK_MINOR_VERSION>=32
    static void spiceUsbDeviceManager_disconnect_finish_callback
                                                    (void*, void*, void*);
#endif

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

#endif // QSPICE_USBDEVICEMANAGER_H
