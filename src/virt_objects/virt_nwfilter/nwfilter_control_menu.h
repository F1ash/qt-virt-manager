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
            QWidget     *parent = nullptr,
            QStringList  params = QStringList(),
            bool         state  = false);

signals:
    void            execMethod(const Act_Param&);

private:
    bool            autoReloadState;
    QAction        *edit = nullptr;
    QAction        *getXMLDesc = nullptr;
    QAction        *reload = nullptr;
    QStringList     parameters;

private slots:
    void            emitExecMethod(QAction*);

};

#endif // VIRT_NWFILTER_CONTROL_MENU_H
