#ifndef PCI_HOST_DEVICE_H
#define PCI_HOST_DEVICE_H

#include "create_widgets/domain/_qwidget.h"
#include <QListWidget>

class PCI_Host_Device : public _QWidget
{
    Q_OBJECT
public:
    explicit PCI_Host_Device(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QListWidget     *devList;
    QVBoxLayout     *commonLayout;

    virErrorPtr      virtErrors;

signals:
    // TODO: write to Log
    void errorMsg(QString);

public slots:
    QDomNodeList getNodeList() const;
    QString      getDevType() const;
    QString      getDevMode() const;
    QString      getDevManaged() const;

private slots:
    void setAvailabledPCIDevices();

    void sendConnErrors();
    void sendGlobalErrors();
};

#endif // PCI_HOST_DEVICE_H
