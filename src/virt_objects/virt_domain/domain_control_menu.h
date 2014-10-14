#ifndef DOMAIN_CONTROL_MENU_H
#define DOMAIN_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QStringList>

class DomainControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit DomainControlMenu(
            QWidget *parent = NULL,
            QStringList params = QStringList(),
            bool state = false);

signals:
    void execMethod(const QStringList&);

private:
    bool            autoReloadState;
    QAction        *start;
    QAction        *pause;
    QAction        *destroy;
    QAction        *edit;
    QAction        *reset;
    QAction        *reboot;
    QAction        *shutdown;
    QAction        *save;
    QAction        *undefine;
    QAction        *autoStart;
    QAction        *getXMLDesc;
    QAction        *migrate;
    QAction        *display;
    QAction        *addToMonitor;
    QAction        *reload;
    QStringList     parameters;

private slots:
    void emitExecMethod(QAction*);

};

#endif // DOMAIN_CONTROL_MENU_H
