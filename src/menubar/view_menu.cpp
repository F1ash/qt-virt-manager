#include "view_menu.h"

ViewMenu::ViewMenu(QWidget *parent) :
    QMenu(parent)
{
    actGroup = new QActionGroup(this);
    actGroup->setExclusive(true);
    hardClassic = addAction("&Hard Classic");
    hardClassic->setCheckable(true);
    hardClassic->setActionGroup(actGroup);
    hardClassic->setShortcut(
                QKeySequence(tr("Ctrl+Shift+H")));
    softTouched = addAction("So&ft Touched");
    softTouched->setCheckable(true);
    softTouched->setActionGroup(actGroup);
    softTouched->setShortcut(
                QKeySequence(tr("Ctrl+Shift+F")));
}
