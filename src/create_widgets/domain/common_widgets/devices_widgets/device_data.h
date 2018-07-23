#ifndef DEVICE_DATA_H
#define DEVICE_DATA_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QStringList>
#include <QScrollArea>
#include <QMessageBox>
#include "device_restore_menu.h"
#include "create_widgets/common/_qwidget.h"

class DeviceData : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceData(
            QWidget        *parent  = Q_NULLPTR,
            virConnectPtr*  conn    = Q_NULLPTR);

signals:
    void             errorMsg(const QString&);
    void             saveDeviceXMLDesc(const int, const QString&);
    void             dataProcessed(bool);

private:
    QLabel          *devName,
                    *devIcon = Q_NULLPTR;
    QPushButton     *save;
    QPushButton     *revert;
    DeviceRestoreMenu
                    *restoreMenu;
    QPushButton     *_close;
    QHBoxLayout     *panelLayout;
    QWidget         *panel;

    _QWidget        *device = Q_NULLPTR;
    QWidget         *scrolled = Q_NULLPTR;
    QVBoxLayout     *infoLayout;
    QScrollArea     *infoWidget;
    QVBoxLayout     *commonLayout;

    bool             changed = false;
    bool             currentStateSaved = true;
    QString          DeviceXMLDesc;
    QString          currentDeviceXMLDesc;
    int              currentItemRow = -1;
    virConnectPtr*   ptr_ConnPtr;
    virNodeDevice  **nodeDevices = Q_NULLPTR;
    virErrorPtr      virtErrors;

public slots:
    QDomDocument     getResult() const;
    void             showDevice(const int, const QString&, const QString&);
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
