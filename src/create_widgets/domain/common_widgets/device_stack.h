#ifndef DEVICE_STACK_H
#define DEVICE_STACK_H

#include <QDialog>
#include <QSettings>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QStringList>
#include <QScrollArea>
#include "create_widgets/domain/_qwidget.h"
#include "create_widgets/domain/domain_widgets.h"
#include <QDebug>

typedef QMap<QString, _QWidget*> WidgetMap;

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
    QWidget         *listWidget;
    QWidget         *buttons;

    virConnectPtr    currWorkConnect = NULL;
    virNodeDevice  **nodeDevices = NULL;
    virErrorPtr      virtErrors;

    QString          currDeviceType;
    WidgetMap        wdgMap;

public slots:
    QDomNodeList getResult() const;

private slots:
    void readNetworkList();
    void readNodeDevicesList();
    void showDevice(QListWidgetItem*);
    void set_Result();

    void sendConnErrors();
    void sendGlobalErrors();

};

#endif // DEVICE_STACK_H
