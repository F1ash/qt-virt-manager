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
    explicit DeviceExistanceMenu(QWidget *parent = 0, bool valid = false);

private:
    bool                 validItem;
    QAction             *addNewDevice;
    QAction             *delSelDevice = NULL;
    Device_Action        result = ERROR;

signals:
    void resultSign(Device_Action);

public slots:

private slots:
    void emitJobSignal();

};

#endif // DEVICE_EXISTANCE_MENU_H
