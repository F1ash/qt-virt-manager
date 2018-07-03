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
            QWidget *parent    = nullptr,
            QVariantMap params = QVariantMap(),
            bool state         = false);

signals:
    void            execMethod(const Act_Param&);

private:
    bool            autoReloadState;
    bool            state, changing;
    QAction        *start = nullptr;
    QAction        *destroy = nullptr;
    QAction        *undefine = nullptr;
    QAction        *changeBegin = nullptr;
    QAction        *changeCommit = nullptr;
    QAction        *changeRollback = nullptr;
    QAction        *getXMLDesc = nullptr;
    QAction        *reload = nullptr;

private slots:
    void            emitExecMethod(QAction*);

};

#endif // VIRT_IFACE_CONTROL_MENU_H
