#ifndef DEVICE_STACK_H
#define DEVICE_STACK_H

#include <QDialog>
#include <QSettings>
#include <QListWidget>
#include <QPushButton>
#include <QStringList>
#include <QScrollArea>
#include "create_widgets/domain/domain_widgets.h"

class DeviceStack : public QDialog
{
    Q_OBJECT
public:
    explicit DeviceStack(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);
    ~DeviceStack();

signals:
    void errorMsg(QString);

private:
    QSettings        settings;
    QStringList      devices;
    QStringList      nets;
    QListWidget     *deviceList;
    QPushButton     *addDevice;
    QPushButton     *cancel;

    QVBoxLayout     *infoLayout;
    QHBoxLayout     *listLayout;
    QHBoxLayout     *buttonlayout;
    QVBoxLayout     *commonLayout;

    QScrollArea     *infoWidget;
    QWidget         *scrolled = NULL;
    QWidget         *listWidget;
    QWidget         *buttons;

    virConnectPtr    currWorkConnect = NULL;
    virNodeDevice  **nodeDevices = NULL;
    virErrorPtr      virtErrors;

    _QWidget        *device = NULL;

public slots:
    QDomDocument getResult() const;

private slots:
    void readNetworkList();
    void readNodeDevicesList();
    void showDevice(QListWidgetItem*);
    void showDevice();
    void set_Result();

    void sendConnErrors();
    void sendGlobalErrors();

};

#endif // DEVICE_STACK_H
