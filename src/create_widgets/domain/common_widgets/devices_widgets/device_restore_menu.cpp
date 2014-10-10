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
    connect(revertData, SIGNAL(hovered()),
            this, SLOT(actionHovered()));
    connect(resetData, SIGNAL(hovered()),
            this, SLOT(actionHovered()));
}

void DeviceRestoreMenu::actionHovered()
{
    if ( sender()==revertData ) {
        this->setToolTip("Revert to previous state");
    } else if ( sender()==resetData ) {
        this->setToolTip("Reset to first state");
    }
}
