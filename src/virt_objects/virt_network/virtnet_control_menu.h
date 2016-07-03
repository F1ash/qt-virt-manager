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
            QWidget     *parent = nullptr,
            QStringList  params = QStringList(),
            bool         state  = false);

signals:
    void            execMethod(const QStringList&);

private:
    bool            autoReloadState;
    QAction        *start = nullptr;
    QAction        *destroy = nullptr;
    QAction        *undefine = nullptr;
    QAction        *autoStart = nullptr;
    QAction        *edit = nullptr;
    QAction        *getXMLDesc = nullptr;
    QAction        *reload = nullptr;
    QStringList     parameters;

private slots:
    void            emitExecMethod(QAction*);

};

#endif // VIRTNET_CONTROL_MENU_H
