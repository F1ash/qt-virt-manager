#ifndef OPEN_FILE_MENU_H
#define OPEN_FILE_MENU_H

#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QIcon>
#include "virt_entity_enums.h"

class OpenFileMenu : public QMenu
{
    Q_OBJECT
public:
    explicit OpenFileMenu(
            QWidget     *parent = nullptr,
            Actions      act    = _NONE_ACTION,
            VIRT_ENTITY  _e     = _NONE_ENTITY);

signals:
    void        fileForMethod(const Act_Param&);

private:
    QIcon       icon;
    QAction    *applyAsIs;
    QAction    *editTemplate;
    QAction    *manual;
    Act_Param   task;

private slots:
    void        chooseExample();
    void        chooseManual();
    void        emitParameters(const QString&, const QString&);

};

#endif // OPEN_FILE_MENU_H
