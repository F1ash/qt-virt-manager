#ifndef VIRT_SECRET_CONTROL_MENU_H
#define VIRT_SECRET_CONTROL_MENU_H

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QStringList>
#include <QDebug>

class VirtSecretControlMenu : public QMenu
{
    Q_OBJECT
public:
    explicit VirtSecretControlMenu(
            QWidget *parent = NULL,
            QStringList params = QStringList(),
            bool state = false);
    ~VirtSecretControlMenu();

signals:
    void            execMethod(const QStringList&);

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
    void            emitExecMethod(QAction*);

};

#endif // VIRT_SECRET_CONTROL_MENU_H
