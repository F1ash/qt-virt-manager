#include "tree_view.h"

#define HNAVY_SIZE 64

TreeView::TreeView(QWidget *parent) :
    QTreeView(parent)
{
    prevL = new HNavigationLabel(this);
    prevL->setToolTip(tr("Previous dock"));
    nextL = new HNavigationLabel(this);
    nextL->setToolTip(tr("Next dock"));
    homeL = new HNavigationLabel(this);
    homeL->setToolTip(tr("To dock page"));
    next = QIcon::fromTheme("next")
            .pixmap(HNAVY_SIZE);
    prev = QIcon::fromTheme("previous")
            .pixmap(HNAVY_SIZE);
    home = QIcon::fromTheme("home")
            .pixmap(HNAVY_SIZE);
    prevL->setPixmap(prev);
    nextL->setPixmap(next);
    homeL->setPixmap(home);
    prevL->setBaseSize(HNAVY_SIZE, HNAVY_SIZE);
    nextL->setBaseSize(HNAVY_SIZE, HNAVY_SIZE);
    homeL->setBaseSize(HNAVY_SIZE, HNAVY_SIZE);
}

void TreeView::resizeEvent(QResizeEvent *ev)
{
    ev->accept();
    prevL->move(
                0,
                ev->size().height()-HNAVY_SIZE);
    homeL->move(
                (ev->size().width()-HNAVY_SIZE)/2,
                ev->size().height()-HNAVY_SIZE);
    nextL->move(
                ev->size().width()-HNAVY_SIZE,
                ev->size().height()-HNAVY_SIZE);
}
