#ifndef DEVICES_H
#define DEVICES_H

#include "create_widgets/domain/_qwidget.h"
#include "device_existance_menu.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>

class Devices : public _QWidget
{
    Q_OBJECT
public:
    explicit Devices(QWidget *parent = 0);
    ~Devices();

signals:

private:
    QListWidget     *usedDeviceList;
    QPushButton     *addNewDevice;
    QWidget         *listWidget;
    QWidget         *infoWidget;

    QVBoxLayout     *listLayout;
    QHBoxLayout     *commonLayout;

public slots:

private slots:
    void choiceNewDevice();
    void showContextMenu(const QPoint&);
    void execDevExistanceMenuResult(Device_Action);
    void detectAttachedDevicesFromXMLDesc();

};

#endif // DEVICES_H
