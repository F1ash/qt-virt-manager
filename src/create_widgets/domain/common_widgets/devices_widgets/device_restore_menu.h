#ifndef DEVICE_RESTORE_MENU_H
#define DEVICE_RESTORE_MENU_H

#include <QMenu>

class DeviceRestoreMenu : public QMenu
{
    Q_OBJECT
public:
    explicit DeviceRestoreMenu(QWidget *parent = Q_NULLPTR);
    QAction    *resetData, *revertData;

signals:

private slots:
    void        actionHovered();

};

#endif // DEVICE_RESTORE_MENU_H
