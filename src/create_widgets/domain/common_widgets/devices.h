#ifndef DEVICES_H
#define DEVICES_H

#include "devices_widgets/device_data.h"
#include "devices_widgets/device_existance_menu.h"
#include "devices_widgets/device_stack.h"
#include <QListWidget>
#include <QPushButton>

class Devices : public _QWidget
{
    Q_OBJECT
public:
    explicit Devices(
            QWidget *parent = 0,
            virConnectPtr conn = NULL,
            virDomainPtr domain = NULL
            );
    ~Devices();

signals:

private:
    QListWidget     *usedDeviceList;
    QPushButton     *addNewDevice;
    QPushButton     *delSelDevice;

    QHBoxLayout     *buttonlayout;
    QVBoxLayout     *listLayout;
    QHBoxLayout     *commonLayout;

    QWidget         *buttons;
    QWidget         *listWidget;
    DeviceData      *infoWidget;

    DeviceStack     *deviceStack = NULL;

public slots:
    QDomDocument getDevDocument() const;

private slots:
    QDomDocument chooseNewDevice();
    void addDevice();
    void delDevice();
    void showDevice();
    void showDevice(int);
    void showContextMenu(const QPoint&);
    void execDevExistanceMenuResult(Device_Action);
    void detectAttachedDevicesFromXMLDesc();
    void saveDeviceXMLDescription(QString&);

};

#endif // DEVICES_H
