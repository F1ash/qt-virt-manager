#ifndef STORAGE_POOL_CONTROL_MENU_H
#define STORAGE_POOL_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QStringList>

class StoragePoolControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit StoragePoolControlMenu(QWidget *parent = 0, QStringList params = QStringList());
    ~StoragePoolControlMenu();

signals:
    void execMethod(const QStringList&);

private:
    QAction        *start;
    QAction        *destroy;
    QAction        *undefine;
    QAction        *autoStart;
    QAction        *getXMLDesc;
    QStringList     parameters;

private slots:
    void emitExecMethod(QAction*);

};

#endif // STORAGE_POOL_CONTROL_MENU_H
