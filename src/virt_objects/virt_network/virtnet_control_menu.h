#ifndef VIRTNET_CONTROL_MENU_H
#define VIRTNET_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QVariantMap>
#include "virt_objects/virt_entity_enums.h"
//#include <QDebug>

class VirtNetControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit VirtNetControlMenu(
            QWidget     *parent = Q_NULLPTR,
            QVariantMap  params = QVariantMap(),
            bool         state  = false);

signals:
    void            execMethod(const Act_Param&);

private:
    bool            autoReloadState;
    bool            active, autostart, persistent;
    QAction        *start = Q_NULLPTR;
    QAction        *destroy = Q_NULLPTR;
    QAction        *undefine = Q_NULLPTR;
    QAction        *autoStart = Q_NULLPTR;
    QAction        *edit = Q_NULLPTR;
    QAction        *getXMLDesc = Q_NULLPTR;
    QAction        *reload = Q_NULLPTR;

private slots:
    void            emitExecMethod(QAction*);

};

#endif // VIRTNET_CONTROL_MENU_H
