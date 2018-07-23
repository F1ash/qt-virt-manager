#ifndef VIRT_SECRET_TOOLBAR_H
#define VIRT_SECRET_TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QPushButton>
#include <QTimerEvent>
#include <QSettings>
#include "virt_objects/virt_entity_enums.h"
//#include <QDebug>

class VirtSecretToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit VirtSecretToolBar(QWidget *parent = Q_NULLPTR);
    ~VirtSecretToolBar();

signals:
    void             execMethod(const Act_Param&);

private:
    QAction         *define_Action;
    QAction         *undefine_Action;
    //QAction         *getXMLDesc_Action;
    QPushButton     *_autoReload;
    QAction         *autoReload;

    QSettings        settings;
    int              interval;
    int              timerId;

private slots:
    void             timerEvent(QTimerEvent*);
    void             detectTriggeredAction(QAction*);
    void             changeAutoReloadState(bool);

public slots:
    Qt::ToolBarArea  get_ToolBarArea(int) const;
    void             enableAutoReload();
    void             stopProcessing();
    bool             getAutoReloadState() const;

};

#endif // VIRT_SECRET_TOOLBAR_H
