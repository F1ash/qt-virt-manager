#ifndef SPICEUSBDEVICEWIDGET_H
#define SPICEUSBDEVICEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QCloseEvent>

class SpiceUsbDeviceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SpiceUsbDeviceWidget(QWidget *parent = 0);

signals:

private:
    QListWidget     *usbDevList;
    QVBoxLayout     *commonLayout;

private slots:
    void             closeEvent(QCloseEvent*);

public slots:
    void             addDevice(QString&);
    void             removeDevice(QString&);
};

#endif // SPICEUSBDEVICEWIDGET_H
