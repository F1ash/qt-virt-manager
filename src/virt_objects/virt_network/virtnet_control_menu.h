#ifndef VIRTNET_CONTROL_MENU_H
#define VIRTNET_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QStringList>

class VirtNetControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit VirtNetControlMenu(QWidget *parent = 0, QStringList params = QStringList(), bool state = false);
    ~VirtNetControlMenu();

signals:
    void execMethod(const QStringList&);

private:
    bool            autoReloadState;
    QAction        *start;
    QAction        *destroy;
    QAction        *undefine;
    QAction        *autoStart;
    QAction        *getXMLDesc;
    QAction        *reload;
    QStringList     parameters;

private slots:
    void emitExecMethod(QAction*);

};

#endif // VIRTNET_CONTROL_MENU_H
