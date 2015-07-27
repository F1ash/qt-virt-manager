#ifndef VIRT_IFACE_CONTROL_MENU_H
#define VIRT_IFACE_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QStringList>
#include <QDebug>

class IfaceControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit IfaceControlMenu(
            QWidget *parent = NULL,
            QStringList params = QStringList(),
            bool state = false);
    ~IfaceControlMenu();

signals:
    void            execMethod(const QStringList&);

private:
    bool            autoReloadState;
    QAction        *start;
    QAction        *destroy;
    QAction        *undefine;
    QAction        *changeBegin;
    QAction        *changeCommit;
    QAction        *changeRollback;
    QAction        *getXMLDesc;
    QAction        *reload;
    QStringList     parameters;

private slots:
    void            emitExecMethod(QAction*);

};

#endif // VIRT_IFACE_CONTROL_MENU_H
