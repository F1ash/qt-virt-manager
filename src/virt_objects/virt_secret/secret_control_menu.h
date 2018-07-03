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
            QWidget     *parent = nullptr,
            QStringList  params = QStringList(),
            bool         state  = false);

signals:
    void            execMethod(const Act_Param&);

private:
    bool            autoReloadState;
    QAction        *start = nullptr;
    QAction        *destroy = nullptr;
    QAction        *undefine = nullptr;
    QAction        *autoStart = nullptr;
    QAction        *getXMLDesc = nullptr;
    QAction        *reload = nullptr;
    QStringList     parameters;

private slots:
    void            emitExecMethod(QAction*);

};

#endif // VIRT_SECRET_CONTROL_MENU_H
