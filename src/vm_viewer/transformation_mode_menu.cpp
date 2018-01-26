#include "transformation_mode_menu.h"

TransformationModeMenu::TransformationModeMenu(
        QWidget *parent, Qt::TransformationMode _mode) :
    QMenu(parent), mode(_mode)
{
    act_group = new QActionGroup(this);
    fast    = addAction(tr("fast (no smoothing)"));
    fast->setCheckable(true);
    fast->setChecked(mode==Qt::FastTransformation);
    fast->setActionGroup(act_group);
    smooth  = addAction(tr("smooth (slower)"));
    smooth->setCheckable(true);
    smooth->setChecked(mode==Qt::SmoothTransformation);
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
        mode = Qt::FastTransformation;
    } else if ( act == smooth ) {
        smooth->setChecked(true);
        mode = Qt::SmoothTransformation;
    };
}

/* public slots */
Qt::TransformationMode TransformationModeMenu::getMode() const
{
    return mode;
}
