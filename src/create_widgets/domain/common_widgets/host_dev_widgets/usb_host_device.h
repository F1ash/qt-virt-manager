#ifndef USB_HOST_DEVICE_H
#define USB_HOST_DEVICE_H

#include "create_widgets/domain/_qwidget.h"
#include <QListWidget>

class USB_Host_Device : public _QWidget
{
    Q_OBJECT
public:
    explicit USB_Host_Device(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QListWidget     *devList;
    QVBoxLayout     *commonLayout;

    virErrorPtr      virtErrors;

signals:
    // TODO: write to Log
    void errorMsg(QString);

public slots:
    QDomDocument getDataDocument() const;

private slots:
    void setAvailabledUSBDevices();

    void sendConnErrors();
    void sendGlobalErrors();
};

#endif // USB_HOST_DEVICE_H
