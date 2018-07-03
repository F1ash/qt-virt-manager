#include "tree_view.h"

#define HNAVY_SIZE 64

TreeView::TreeView(QWidget *parent) :
    QTreeView(parent)
{
    prevL = new HNavigationLabel(this, DIRECT::PREV);
    prevL->setToolTip(tr("Previous dock"));
    nextL = new HNavigationLabel(this, DIRECT::NEXT);
    nextL->setToolTip(tr("Next dock"));
    homeL = new HNavigationLabel(this, DIRECT::NONE);
    homeL->setToolTip(tr("To dock page"));
    loadL = new WaitLoadLabel(this);

    next = QIcon::fromTheme("next")
            .pixmap(HNAVY_SIZE);
    prev = QIcon::fromTheme("previous")
            .pixmap(HNAVY_SIZE);
    home = QIcon::fromTheme("home")
            .pixmap(HNAVY_SIZE);
    load = QIcon::fromTheme("radar")
            .pixmap(HNAVY_SIZE*4);

    prevL->setPixmap(prev);
    nextL->setPixmap(next);
    homeL->setPixmap(home);
    loadL->setPixmap(load);

    prevL->setBaseSize(HNAVY_SIZE, HNAVY_SIZE);
    nextL->setBaseSize(HNAVY_SIZE, HNAVY_SIZE);
    homeL->setBaseSize(HNAVY_SIZE, HNAVY_SIZE);
    loadL->setFixedSize(HNAVY_SIZE*4, HNAVY_SIZE*4);
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
    loadL->move(
                (ev->size().width()-HNAVY_SIZE*4)/2,
                (ev->size().height()-HNAVY_SIZE*4)/2);
}
