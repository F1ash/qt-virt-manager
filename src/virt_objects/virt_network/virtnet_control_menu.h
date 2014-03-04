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
    explicit VirtNetControlMenu(QWidget *parent = 0, QStringList params = QStringList());
    ~VirtNetControlMenu();

signals:
    void execMethod(const QStringList&);

private:
    QAction        *start;
    QAction        *destroy;
    QAction        *undefine;
    QAction        *autoStart;
    QAction        *getXMLDesc;
    QStringList     parameters;

private slots:
    void emitExecMethod(QAction*);

};

#endif // VIRTNET_CONTROL_MENU_H
