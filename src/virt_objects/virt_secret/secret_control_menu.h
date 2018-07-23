#ifndef VIRT_SECRET_CONTROL_MENU_H
#define VIRT_SECRET_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QStringList>
#include "virt_objects/virt_entity_enums.h"
//#include <QDebug>

class VirtSecretControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit VirtSecretControlMenu(
            QWidget     *parent = Q_NULLPTR,
            QStringList  params = QStringList(),
            bool         state  = false);

signals:
    void            execMethod(const Act_Param&);

private:
    bool            autoReloadState;
    QAction        *start = Q_NULLPTR;
    QAction        *destroy = Q_NULLPTR;
    QAction        *undefine = Q_NULLPTR;
    QAction        *autoStart = Q_NULLPTR;
    QAction        *getXMLDesc = Q_NULLPTR;
    QAction        *reload = Q_NULLPTR;
    QStringList     parameters;

private slots:
    void            emitExecMethod(QAction*);

};

#endif // VIRT_SECRET_CONTROL_MENU_H
