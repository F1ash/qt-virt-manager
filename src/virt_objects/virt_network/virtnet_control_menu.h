#ifndef VIRTNET_CONTROL_MENU_H
#define VIRTNET_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QStringList>
#include <QDebug>

class VirtNetControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit VirtNetControlMenu(
            QWidget     *parent = NULL,
            QStringList  params = QStringList(),
            bool         state  = false);

signals:
    void execMethod(const QStringList&);

private:
    bool            autoReloadState;
    QAction        *start = NULL;
    QAction        *destroy = NULL;
    QAction        *undefine = NULL;
    QAction        *autoStart = NULL;
    QAction        *getXMLDesc = NULL;
    QAction        *reload = NULL;
    QStringList     parameters;

private slots:
    void            emitExecMethod(QAction*);

};

#endif // VIRTNET_CONTROL_MENU_H
