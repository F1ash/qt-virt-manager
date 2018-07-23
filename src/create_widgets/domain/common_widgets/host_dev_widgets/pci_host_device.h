#ifndef PCI_HOST_DEVICE_H
#define PCI_HOST_DEVICE_H

#include "create_widgets/common/_qwidget.h"
#include <QListWidget>
#include "virt_objects/_virt_thread.h"

class pci_hostHlpThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit pci_hostHlpThread(
            QObject        *parent      = Q_NULLPTR,
            virConnectPtr*  connPtrPtr  = Q_NULLPTR);
    void             run();
signals:
    void             result(QStringList&);
};

class PCI_Host_Device : public _QWidget
{
    Q_OBJECT
public:
    explicit PCI_Host_Device(
            QWidget        *parent      = Q_NULLPTR,
            virConnectPtr*  connPtrPtr  = Q_NULLPTR);

private:
    QListWidget     *devList;
    QVBoxLayout     *commonLayout;

    pci_hostHlpThread
                    *hlpThread;

public slots:
    QDomDocument     getDataDocument() const;

private slots:
    void             setAvailabledPCIDevices(QStringList&);
    void             emitCompleteSignal();
};

#endif // PCI_HOST_DEVICE_H
