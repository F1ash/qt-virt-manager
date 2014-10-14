#ifndef RESTORE_PANEL_H
#define RESTORE_PANEL_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include "common_widgets/devices_widgets/device_restore_menu.h"

class RestorePanel : public QWidget
{
    Q_OBJECT
public:
    explicit RestorePanel(QWidget *parent = NULL);

signals:
    void             saveData();
    void             revertData();
    void             resetData();

private:
    QPushButton     *save;
    QPushButton     *revert;
    DeviceRestoreMenu
                    *restoreMenu;
    QHBoxLayout     *panelLayout;

public slots:
    void             stateChanged();
    void             stateChanged(bool);
};

#endif // RESTORE_PANEL_H
