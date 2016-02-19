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
            QWidget *parent    = nullptr,
            QStringList params = QStringList(),
            bool state         = false);

signals:
    void            execMethod(const QStringList&);

private:
    bool            autoReloadState;
    QAction        *start = nullptr;
    QAction        *destroy = nullptr;
    QAction        *undefine = nullptr;
    QAction        *changeBegin = nullptr;
    QAction        *changeCommit = nullptr;
    QAction        *changeRollback = nullptr;
    QAction        *getXMLDesc = nullptr;
    QAction        *reload = nullptr;
    QStringList     parameters;

private slots:
    void            emitExecMethod(QAction*);

};

#endif // VIRT_IFACE_CONTROL_MENU_H
