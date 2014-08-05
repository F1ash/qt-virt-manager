#ifndef DEVICES_H
#define DEVICES_H

#include "device_existance_menu.h"
#include "device_stack.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>

class Devices : public _QWidget
{
    Q_OBJECT
public:
    explicit Devices(
            QWidget *parent = 0,
            virConnectPtr conn = NULL,
            virDomainPtr domain = NULL);
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
    QWidget         *infoWidget;

    virConnect      *currWorkConnect = NULL;
    virDomain       *currDomain = NULL;

    DeviceStack     *deviceStack = NULL;
    WidgetMap        wdgMap;

public slots:
    QDomNodeList getNodeList() const;

private slots:
    QDomDocument choiceNewDevice();
    void addDevice();
    void delDevice();
    void showContextMenu(const QPoint&);
    void execDevExistanceMenuResult(Device_Action);
    void detectAttachedDevicesFromXMLDesc();

};

#endif // DEVICES_H
