#ifndef VIRT_NWFILTER_TOOLBAR_H
#define VIRT_NWFILTER_TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QPushButton>
#include <QTimerEvent>
#include <QSettings>
#include <QDebug>

class VirtNWFilterToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit VirtNWFilterToolBar(QWidget *parent = nullptr);
    ~VirtNWFilterToolBar();

signals:
    void             execMethod(const QStringList&);

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
    void             detectTriggerredAction(QAction*);
    void             changeAutoReloadState(bool);

public slots:
    Qt::ToolBarArea  get_ToolBarArea(int) const;
    void             enableAutoReload();
    void             stopProcessing();
    bool             getAutoReloadState() const;

};

#endif // VIRT_NWFILTER_TOOLBAR_H
