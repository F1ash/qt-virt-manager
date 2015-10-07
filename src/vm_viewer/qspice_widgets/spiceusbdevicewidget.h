#ifndef SPICEUSBDEVICEWIDGET_H
#define SPICEUSBDEVICEWIDGET_H

#include <QDialog>
#include <QVBoxLayout>
#include <QListWidget>
#include <QDebug>

class SpiceUsbDeviceWidget : public QDialog
{
    Q_OBJECT
public:
    explicit SpiceUsbDeviceWidget(QWidget *parent = 0);

signals:
    void             connectDevice(QString&);
    void             disconnectDevice(QString&);
    void             devicesChanged(void*);

private:
    QListWidget     *usbDevList;
    QVBoxLayout     *commonLayout;

private slots:
    void             changeDeviceState(QListWidgetItem*);

public slots:
    void             addItem(QListWidgetItem*);
    void             clearList();
    void             addDevice(QString&);
    void             removeDevice(QString&);
};

#endif // SPICEUSBDEVICEWIDGET_H
