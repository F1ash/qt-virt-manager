#ifndef DOMAIN_CONTROL_MENU_H
#define DOMAIN_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QVariantMap>
#include "virt_objects/virt_entity_enums.h"
//#include <QDebug>

class DomainControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit DomainControlMenu(
            QWidget     *parent = Q_NULLPTR,
            QVariantMap  params = QVariantMap(),
            bool         state  = false);

signals:
    void            execMethod(const Act_Param&);

private:
    bool            autoReloadState;
    bool            active, autostart, persistent;
    QAction        *start = Q_NULLPTR;
    QAction        *pause = Q_NULLPTR;
    QAction        *destroy = Q_NULLPTR;
    QAction        *edit = Q_NULLPTR;
    QAction        *reset = Q_NULLPTR;
    QAction        *reboot = Q_NULLPTR;
    QAction        *shutdown = Q_NULLPTR;
    QAction        *save = Q_NULLPTR;
    QAction        *undefine = Q_NULLPTR;
    QAction        *autoStart = Q_NULLPTR;
    QAction        *getXMLDesc = Q_NULLPTR;
    QAction        *RunningData = Q_NULLPTR;
    QAction        *InactiveData = Q_NULLPTR;
    QMenu          *xmlDescParams = Q_NULLPTR;
    QAction        *migrate = Q_NULLPTR;
    QAction        *display = Q_NULLPTR;
    QAction        *displayInExternal = Q_NULLPTR;
    QAction        *addToMonitor = Q_NULLPTR;
    QAction        *reload = Q_NULLPTR;
    QAction        *createSnapshot = Q_NULLPTR;
    QAction        *moreSnapshot_Actions = Q_NULLPTR;

private slots:
    void            emitExecMethod(QAction*);

};

#endif // DOMAIN_CONTROL_MENU_H
