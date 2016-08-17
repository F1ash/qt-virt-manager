#ifndef DEVICES_H
#define DEVICES_H

#include "devices_widgets/device_data.h"
#include "devices_widgets/device_existance_menu.h"
#include "devices_widgets/device_stack.h"
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>

#define DEVICE_ORDER QStringList()\
    <<"Emulator"<<"Disk"<<"Filesystem"<<"Controller"\
    <<"Host Device"<<"USB Redirector"<<"SmartCard"\
    <<"Network"<<"Input"<<"Hub"<<"Display"<<"Video"\
    <<"Console"<<"Serial Port"<<"Parallel Port"\
    <<"Channel"<<"Sound"<<"WatchDog"<<"MemBalloon"\
    <<"RNG"<<"TPM"<<"NVRAM"<<"Panic"

class Devices : public _QWidget
{
    Q_OBJECT
public:
    explicit Devices(
            QWidget        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr,
            QString         _xmlDesc    = QString());

signals:
    void             devicesChanged(QDomDocument&);

private:
    QListWidget     *usedDeviceList;
    QPushButton     *addNewDevice;
    QPushButton     *delSelDevice;

    QHBoxLayout     *buttonlayout;
    QWidget         *buttons;
    QVBoxLayout     *listLayout;
    QWidget         *listWidget;
    DeviceData      *infoWidget;
    QHBoxLayout     *baseLayout;
    QWidget         *baseWdg;

    QStackedWidget  *stackWdg;
    QHBoxLayout     *commonLayout;

    DeviceStack     *deviceStack;

    const QString    xmlDesc;
    const QStringList
                     devNameOrder = DEVICE_ORDER;

public slots:
    QDomDocument     getDataDocument() const;
    void             setEmulator(const QString&);
    void             initBootDevices();

private slots:
    void             showDeviceStack();
    void             addDevice();
    void             addDeviceToUsedDevList(QDomDocument &doc, const bool flag = true);
    void             delDevice();
    void             showDevice();
    void             showDevice(QListWidgetItem*,QListWidgetItem*);
    void             showContextMenu(const QPoint&);
    void             execDevExistanceMenuResult(Device_Action);
    void             detectAttachedDevicesFromXMLDesc();
    void             saveDeviceXMLDescription(const int, const QString&);

};

#endif // DEVICES_H
