#include "qspice-usbdevice-widget.h"

QSpiceUsbDeviceWidget::QSpiceUsbDeviceWidget(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("USB Redirection"));
    settings.beginGroup("USB Redirection");
    restoreGeometry(settings.value("Geometry").toByteArray());
    settings.endGroup();
    usbDevList = new QListWidget(this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(usbDevList);
    setLayout(commonLayout);
    setMinimumSize(100, 100);
    setContentsMargins(0,0,0,0);
    connect(usbDevList, SIGNAL(itemChanged(QListWidgetItem*)),
            this, SLOT(changeDeviceState(QListWidgetItem*)));
}
QSpiceUsbDeviceWidget::~QSpiceUsbDeviceWidget()
{
    settings.beginGroup("USB Redirection");
    settings.setValue("Geometry", saveGeometry());
    settings.endGroup();
}

/* private slots */
void QSpiceUsbDeviceWidget::changeDeviceState(QListWidgetItem *item)
{
    if ( item==Q_NULLPTR ) return;
    QString _id = QString("%1 %2")
            .arg(item->text()).arg(item->data(Qt::UserRole).toString());
    //qDebug()<<item->text()<<item->checkState();
    if ( item->checkState()==Qt::Checked )
        emit connectDevice(_id);
    else
        emit disconnectDevice(_id);
}

/* public slots */
void QSpiceUsbDeviceWidget::addItem(QListWidgetItem *item)
{
    usbDevList->addItem(item);
}

void QSpiceUsbDeviceWidget::clearList()
{
    usbDevList->clear();
}

void QSpiceUsbDeviceWidget::addDevice(QString &dev)
{
    Q_UNUSED(dev);
    emit devicesChanged(static_cast<void*>(this));
}

void QSpiceUsbDeviceWidget::removeDevice(QString &dev)
{
    Q_UNUSED(dev);
    emit devicesChanged(static_cast<void*>(this));
}
