#ifndef DOMAIN_CONTROL_MENU_H
#define DOMAIN_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QStringList>
#include <QDebug>

class DomainControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit DomainControlMenu(
            QWidget     *parent = NULL,
            QStringList  params = QStringList(),
            bool         state  = false);

signals:
    void            execMethod(const QStringList&);

private:
    bool            autoReloadState;
    QAction        *start = NULL;
    QAction        *pause = NULL;
    QAction        *destroy = NULL;
    QAction        *edit = NULL;
    QAction        *reset = NULL;
    QAction        *reboot = NULL;
    QAction        *shutdown = NULL;
    QAction        *save = NULL;
    QAction        *undefine = NULL;
    QAction        *autoStart = NULL;
    QAction        *getXMLDesc = NULL;
    QAction        *migrate = NULL;
    QAction        *display = NULL;
    QAction        *addToMonitor = NULL;
    QAction        *reload = NULL;
    QAction        *createSnapshot = NULL;
    QAction        *moreSnapshot_Actions = NULL;
    QStringList     parameters;

private slots:
    void            emitExecMethod(QAction*);

};

#endif // DOMAIN_CONTROL_MENU_H
