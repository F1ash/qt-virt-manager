#ifndef QSPICE_USBDEVICE_WIDGET_H
#define QSPICE_USBDEVICE_WIDGET_H

#if !defined(QSPICE_WIDGET_H_INSIDE) && !defined(QSPICE_WIDGET_COMPILATION)
#warning "Only <qspice-widget.h> can be included directly"
#endif

#include <QDialog>
#include <QVBoxLayout>
#include <QListWidget>
#include <QSettings>
//#include <QDebug>

class QSpiceUsbDeviceWidget : public QDialog
{
    Q_OBJECT
public:
    explicit QSpiceUsbDeviceWidget(QWidget *parent = Q_NULLPTR);
    ~QSpiceUsbDeviceWidget();

signals:
    void             connectDevice(QString&);
    void             disconnectDevice(QString&);
    void             devicesChanged(void*);

private:
    QSettings        settings;
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

#endif // QSPICE_USBDEVICE_WIDGET_H
