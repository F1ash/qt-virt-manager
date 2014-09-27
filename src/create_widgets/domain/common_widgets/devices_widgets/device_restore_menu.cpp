#include "device_restore_menu.h"

DeviceRestoreMenu::DeviceRestoreMenu(QWidget *parent) :
    QMenu(parent)
{
    revertData = addAction(
                QIcon::fromTheme("document-revert"),
                "Revert");
    revertData->setToolTip("Revert to previous state");
    addSeparator();
    resetData = addAction(
                QIcon::fromTheme("go-first"),
                "Reset");
    resetData->setToolTip("Reset to first state");
}
