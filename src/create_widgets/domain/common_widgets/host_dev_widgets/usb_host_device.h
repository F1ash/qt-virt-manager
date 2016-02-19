#ifndef USB_HOST_DEVICE_H
#define USB_HOST_DEVICE_H

#include "create_widgets/domain/_qwidget.h"
#include <QListWidget>
#include "virt_objects/_virt_thread.h"

class usb_hostHlpThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit usb_hostHlpThread(
            QObject        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr);
    void             run();
signals:
    void             result(QStringList&);
};

class USB_Host_Device : public _QWidget
{
    Q_OBJECT
public:
    explicit USB_Host_Device(
            QWidget        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr);

private:
    QListWidget     *devList;
    QVBoxLayout     *commonLayout;

    usb_hostHlpThread
                    *hlpThread;

public slots:
    QDomDocument     getDataDocument() const;

private slots:
    void             setAvailabledUSBDevices(QStringList&);
};

#endif // USB_HOST_DEVICE_H
