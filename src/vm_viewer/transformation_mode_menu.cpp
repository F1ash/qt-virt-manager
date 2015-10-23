#include "transformation_mode_menu.h"

TransformationModeMenu::TransformationModeMenu(QWidget *parent) :
    QMenu(parent)
{
    act_group = new QActionGroup(this);
    fast    = addAction("fast (no smoothing)");
    fast->setCheckable(true);
    fast->setChecked(false);
    fast->setActionGroup(act_group);
    smooth  = addAction("smooth (slower)");
    smooth->setCheckable(true);
    smooth->setChecked(true);
    smooth->setActionGroup(act_group);
    act_group->setExclusive(true);
    connect(this, SIGNAL(triggered(QAction*)),
            this, SLOT(emitNewMode(QAction*)));
}

/* private slots */
void TransformationModeMenu::emitNewMode(QAction *act)
{
    if ( act == fast ) {
        fast->setChecked(true);
        emit new_mode(Qt::FastTransformation);
    } else if ( act == smooth ) {
        smooth->setChecked(true);
        emit new_mode(Qt::SmoothTransformation);
    };
}
