#ifndef DEVICE_EXISTANCE_MENU_H
#define DEVICE_EXISTANCE_MENU_H

#include <QMenu>

enum Device_Action {
    ERROR = -1,
    ADD,
    DEL
};

class DeviceExistanceMenu : public QMenu
{
    Q_OBJECT
public:
    explicit DeviceExistanceMenu(
            QWidget *parent = Q_NULLPTR,
            bool     valid = false);

private:
    bool             validItem;
    QAction         *addNewDevice;
    QAction         *delSelDevice = Q_NULLPTR;
    Device_Action    result = ERROR;

signals:
    void             resultSign(Device_Action);

public slots:

private slots:
    void emitJobSignal();

};

#endif // DEVICE_EXISTANCE_MENU_H
