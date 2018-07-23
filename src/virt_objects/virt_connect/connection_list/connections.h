#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include <QMainWindow>
#include "conn_list_widget.h"
#include "toolbar.h"

class Connections : public QMainWindow
{
    Q_OBJECT
public:
    explicit Connections(QWidget *parent = Q_NULLPTR);
    ConnectionList      *list;
    ToolBar             *toolBar;
    void                 setToolBarArea(int);

signals:

public slots:
};

#endif // CONNECTIONS_H
