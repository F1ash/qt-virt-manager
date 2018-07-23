#ifndef VIRT_NWFILTER_CONTROL_MENU_H
#define VIRT_NWFILTER_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QStringList>
#include "virt_objects/virt_entity_enums.h"
//#include <QDebug>

class VirtNWFilterControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit VirtNWFilterControlMenu(
            QWidget     *parent = Q_NULLPTR,
            QStringList  params = QStringList(),
            bool         state  = false);

signals:
    void            execMethod(const Act_Param&);

private:
    bool            autoReloadState;
    QAction        *edit = Q_NULLPTR;
    QAction        *getXMLDesc = Q_NULLPTR;
    QAction        *reload = Q_NULLPTR;
    QStringList     parameters;

private slots:
    void            emitExecMethod(QAction*);

};

#endif // VIRT_NWFILTER_CONTROL_MENU_H
