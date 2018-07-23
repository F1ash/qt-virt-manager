#ifndef _LIST_WIDGET_H
#define _LIST_WIDGET_H

#include "_checked_widget.h"
#include <QListWidget>
#include <QPushButton>

class _List_Widget : public _Checked_Widget
{
    Q_OBJECT
public:
    explicit _List_Widget(
            QWidget *parent = Q_NULLPTR,
            QString  tag    = "");
    QListWidget     *list;
    QHBoxLayout     *panelLayout;

private:
    QPushButton     *add;
    QPushButton     *del;
    QWidget         *panel;

public slots:
    virtual void     addItem();
    virtual void     delItem();
};

#endif // _LIST_WIDGET_H
