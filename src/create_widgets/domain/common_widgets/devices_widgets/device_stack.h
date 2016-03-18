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
            QWidget        *parent  = nullptr,
            virConnectPtr*  conn    = nullptr);

signals:
    void             errorMsg(QString&);
    void             ptrIsNull();

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
    QWidget         *scrolled = nullptr;
    QWidget         *listWidget;
    QWidget         *buttons;

    virConnectPtr*   ptr_ConnPtr;
    virErrorPtr      virtErrors;

    _QWidget        *device = nullptr;

public slots:
    QDomDocument     getResult() const;
    void             clearDevice();

private slots:
    void             showDevice(QListWidgetItem*);
    void             showDevice();
    void             set_Result();
    void             deviceDataProcessed();
};

#endif // DEVICE_STACK_H
