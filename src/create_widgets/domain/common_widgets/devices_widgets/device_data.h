#ifndef DEVICE_DATA_H
#define DEVICE_DATA_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QStringList>
#include <QScrollArea>
#include <QMessageBox>
#include "create_widgets/domain/domain_widgets.h"
#include "device_restore_menu.h"

class DeviceData : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceData(
            QWidget        *parent  = nullptr,
            virConnectPtr*  conn    = nullptr);

signals:
    void             errorMsg(QString&);
    void             saveDeviceXMLDesc(int, QString&);
    void             dataProcessed(bool);

private:
    QLabel          *devName;
    QPushButton     *save;
    QPushButton     *revert;
    DeviceRestoreMenu
                    *restoreMenu;
    QPushButton     *_close;
    QHBoxLayout     *panelLayout;
    QWidget         *panel;

    _QWidget        *device = nullptr;
    QWidget         *scrolled = nullptr;
    QVBoxLayout     *infoLayout;
    QScrollArea     *infoWidget;
    QVBoxLayout     *commonLayout;

    bool             changed = false;
    bool             currentStateSaved = true;
    QString          DeviceXMLDesc;
    QString          currentDeviceXMLDesc;
    int              currentItemRow = -1;
    virConnectPtr*   ptr_ConnPtr;
    virNodeDevice  **nodeDevices = nullptr;
    virErrorPtr      virtErrors;

public slots:
    QDomDocument     getResult() const;
    void             showDevice(int, QString&, QString&);
    void             closeDataEdit();
    void             clearDataEdit();

private slots:
    void             currentStateChanged();
    void             saveDeviceData();
    void             revertDeviceData();
    void             setStartState();
    void             deviceDataProcessed();
};

#endif // DEVICE_DATA_H
