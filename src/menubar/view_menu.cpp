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
    connect(hardClassic, SIGNAL(toggled(bool)),
            this, SLOT(viewModeChanged()));
    connect(softTouched, SIGNAL(toggled(bool)),
            this, SLOT(viewModeChanged()));
}
void ViewMenu::viewModeChanged()
{
    if        ( sender()==hardClassic ) {
        hardClassic->setEnabled(false);
        softTouched->setEnabled(true);
    } else if ( sender()==softTouched ) {
        hardClassic->setEnabled(true);
        softTouched->setEnabled(false);
    } else {
        hardClassic->setEnabled(true);
        softTouched->setEnabled(true);
    };
}
