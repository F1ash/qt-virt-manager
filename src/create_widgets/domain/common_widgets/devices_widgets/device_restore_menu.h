#ifndef DEVICE_RESTORE_MENU_H
#define DEVICE_RESTORE_MENU_H

#include <QMenu>

class DeviceRestoreMenu : public QMenu
{
    Q_OBJECT
public:
    explicit DeviceRestoreMenu(QWidget *parent = 0);
    QAction         *resetData, *revertData;

signals:

public slots:

};

#endif // DEVICE_RESTORE_MENU_H
