#ifndef FILE_MENU_H
#define FILE_MENU_H

#include <QMenu>

class FileMenu : public QMenu
{
    Q_OBJECT
public:
    explicit FileMenu(QWidget *parent = nullptr);
    QAction     *newConn;
    QAction     *hideToTray;
    QAction     *restart;
    QAction     *exitApp;
};

#endif // FILE_MENU_H
