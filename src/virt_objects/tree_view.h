#ifndef TREE_VIEW_H
#define TREE_VIEW_H

#include <QTreeView>
#include <QPixmap>
#include <QResizeEvent>
#include "h_navigation_label.h"

class TreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit TreeView(QWidget *parent = nullptr);
    HNavigationLabel *prevL, *nextL, *homeL;

private:
    QPixmap           next, prev, home;

private slots:
    void              resizeEvent(QResizeEvent*);
};

#endif // TREE_VIEW_H
