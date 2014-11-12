#ifndef DEVICES_H
#define DEVICES_H

#include "devices_widgets/device_data.h"
#include "devices_widgets/device_existance_menu.h"
#include "devices_widgets/device_stack.h"
#include <QListWidget>
#include <QPushButton>

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
            QWidget *parent = NULL,
            virConnectPtr conn = NULL,
            QString _xmlDesc = QString());
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

    const QString    xmlDesc;
    const QStringList
                     devNameOrder = DEVICE_ORDER;

public slots:
    QDomDocument     getDataDocument() const;
    void             setEmulator(QString&);

private slots:
    QDomDocument     chooseNewDevice();
    void             addDevice();
    void             addDeviceToUsedDevList(QDomDocument&);
    void             delDevice();
    void             showDevice();
    void             showDevice(QListWidgetItem*,QListWidgetItem*);
    void             showContextMenu(const QPoint&);
    void             execDevExistanceMenuResult(Device_Action);
    void             detectAttachedDevicesFromXMLDesc();
    void             saveDeviceXMLDescription(QString&);

};

#endif // DEVICES_H
