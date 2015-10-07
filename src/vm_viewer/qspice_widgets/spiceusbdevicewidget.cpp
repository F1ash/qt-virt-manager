#include "spiceusbdevicewidget.h"

SpiceUsbDeviceWidget::SpiceUsbDeviceWidget(QWidget *parent) :
    QDialog(parent)
{
    usbDevList = new QListWidget(this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(usbDevList);
    setLayout(commonLayout);
    setMinimumSize(100, 100);
    setContentsMargins(0,0,0,0);
    connect(usbDevList, SIGNAL(itemChanged(QListWidgetItem*)),
            this, SLOT(changeDeviceState(QListWidgetItem*)));
}

/* private slots */
void SpiceUsbDeviceWidget::changeDeviceState(QListWidgetItem *item)
{
    QString _id = QString("%1 %2")
            .arg(item->text()).arg(item->data(Qt::UserRole).toString());
    //qDebug()<<item->text()<<item->checkState();
    if ( item->checkState()==Qt::Checked )
        emit connectDevice(_id);
    else
        emit disconnectDevice(_id);
}

/* public slots */
void SpiceUsbDeviceWidget::addItem(QListWidgetItem *item)
{
    usbDevList->addItem(item);
}

void SpiceUsbDeviceWidget::clearList()
{
    usbDevList->clear();
}

void SpiceUsbDeviceWidget::addDevice(QString &dev)
{
    Q_UNUSED(dev);
    emit devicesChanged((void*)this);
}

void SpiceUsbDeviceWidget::removeDevice(QString &dev)
{
    Q_UNUSED(dev);
    emit devicesChanged((void*)this);
}
