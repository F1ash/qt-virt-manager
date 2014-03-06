#ifndef STORAGE_VOL_CONTROL_MENU_H
#define STORAGE_VOL_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QStringList>

class StorageVolControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit StorageVolControlMenu(QWidget *parent = 0, QStringList params = QStringList());
    ~StorageVolControlMenu();

signals:
    void execMethod(const QStringList&);

private:
    QAction        *start;
    QAction        *destroy;
    QAction        *undefine;
    QAction        *autoStart;
    QAction        *getXMLDesc;
    QAction        *overview;
    QStringList     parameters;

private slots:
    void emitExecMethod(QAction*);

};

#endif // STORAGE_VOL_CONTROL_MENU_H
