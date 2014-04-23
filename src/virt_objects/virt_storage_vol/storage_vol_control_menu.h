#ifndef STORAGE_VOL_CONTROL_MENU_H
#define STORAGE_VOL_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QStringList>
#include <QDebug>

class StorageVolControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit StorageVolControlMenu(QWidget *parent = 0, QStringList params = QStringList(), bool state = false);
    ~StorageVolControlMenu();

signals:
    void execMethod(const QStringList&);

private:
    bool            autoReloadState;
    QAction        *start;
    QAction        *destroy;
    QAction        *undefine;
    QAction        *autoStart;
    QAction        *getXMLDesc;
    QAction        *reload;
    QStringList     parameters;

private slots:
    void emitExecMethod(QAction*);

};

#endif // STORAGE_VOL_CONTROL_MENU_H
