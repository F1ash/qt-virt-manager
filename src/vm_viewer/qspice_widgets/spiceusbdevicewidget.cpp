#include "spiceusbdevicewidget.h"

SpiceUsbDeviceWidget::SpiceUsbDeviceWidget(QWidget *parent) : QWidget(parent)
{
    usbDevList = new QListWidget(this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(usbDevList);
    setLayout(commonLayout);
}

/* private slots */
void SpiceUsbDeviceWidget::closeEvent(QCloseEvent *ev)
{
    ev->ignore();
    hide();
}

/* public slots */
void SpiceUsbDeviceWidget::addDevice(QString &dev)
{

}

void SpiceUsbDeviceWidget::removeDevice(QString &dev)
{

}
