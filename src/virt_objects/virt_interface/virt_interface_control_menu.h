#ifndef VIRT_IFACE_CONTROL_MENU_H
#define VIRT_IFACE_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QVariantMap>
#include "virt_objects/virt_entity_enums.h"
//#include <QDebug>

class IfaceControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit IfaceControlMenu(
            QWidget *parent    = Q_NULLPTR,
            QVariantMap params = QVariantMap(),
            bool state         = false);

signals:
    void            execMethod(const Act_Param&);

private:
    bool            autoReloadState;
    bool            state, changing;
    QAction        *start = Q_NULLPTR;
    QAction        *destroy = Q_NULLPTR;
    QAction        *undefine = Q_NULLPTR;
    QAction        *changeBegin = Q_NULLPTR;
    QAction        *changeCommit = Q_NULLPTR;
    QAction        *changeRollback = Q_NULLPTR;
    QAction        *getXMLDesc = Q_NULLPTR;
    QAction        *reload = Q_NULLPTR;

private slots:
    void            emitExecMethod(QAction*);

};

#endif // VIRT_IFACE_CONTROL_MENU_H
