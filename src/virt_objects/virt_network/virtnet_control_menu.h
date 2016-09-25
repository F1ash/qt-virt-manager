#ifndef VIRTNET_CONTROL_MENU_H
#define VIRTNET_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QVariantMap>
#include "virt_objects/virt_entity_enums.h"
#include <QDebug>

class VirtNetControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit VirtNetControlMenu(
            QWidget     *parent = nullptr,
            QVariantMap  params = QVariantMap(),
            bool         state  = false);

signals:
    void            execMethod(const Act_Param&);

private:
    bool            autoReloadState;
    bool            active, autostart, persistent;
    QAction        *start = nullptr;
    QAction        *destroy = nullptr;
    QAction        *undefine = nullptr;
    QAction        *autoStart = nullptr;
    QAction        *edit = nullptr;
    QAction        *getXMLDesc = nullptr;
    QAction        *reload = nullptr;

private slots:
    void            emitExecMethod(QAction*);

};

#endif // VIRTNET_CONTROL_MENU_H
