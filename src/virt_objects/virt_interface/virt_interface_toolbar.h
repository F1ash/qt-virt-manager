#ifndef VIRT_INTERFACE_TOOLBAR_H
#define VIRT_INTERFACE_TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QPushButton>
#include <QTimerEvent>
#include <QSettings>
#include <QDebug>

class InterfaceToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit InterfaceToolBar(QWidget *parent = NULL);
    ~InterfaceToolBar();

signals:
    void             fileForMethod(const QStringList&);
    void             execMethod(const QStringList&);

private:
    QAction         *start_Action;
    QAction         *destroy_Action;
    QAction         *define_Action;
    QAction         *undefine_Action;
    QAction         *changeBegin_Action;
    QAction         *changeCommit_Action;
    QAction         *changeRollback_Action;
    QAction         *getXMLDesc_Action;
    QPushButton     *_autoReload;
    QAction         *autoReload;

    QSettings        settings;
    int              interval;
    int              timerId;

private slots:
    void             timerEvent(QTimerEvent*);
    void             detectTriggerredAction(QAction*);
    void             changeAutoReloadState(bool);

public slots:
    Qt::ToolBarArea  get_ToolBarArea(int) const;
    void             enableAutoReload();
    void             stopProcessing();
    bool             getAutoReloadState() const;

};

#endif // VIRT_INTERFACE_TOOLBAR_H
