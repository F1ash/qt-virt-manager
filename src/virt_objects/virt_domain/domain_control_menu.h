#ifndef VIRTNET_CONTROL_MENU_H
#define VIRTNET_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QStringList>

class DomainControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit DomainControlMenu(QWidget *parent = 0, QStringList params = QStringList());
    ~DomainControlMenu();

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
