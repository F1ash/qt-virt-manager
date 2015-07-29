#ifndef DELETE_POOL_MENU_H
#define DELETE_POOL_MENU_H

#include <QMenu>
#include <libvirt/libvirt.h>

class Delete_Pool_Menu : public QMenu
{
    Q_OBJECT
public:
    explicit Delete_Pool_Menu(QWidget *parent = 0);

signals:
    void             execMethod(const QStringList&);

private:
    QAction         *delete_Normal;
    QAction         *delete_Zeroed;

private slots:
    void             emitExecMethod(QAction*);
    void             showActionToolTip(QAction*);
};

#endif // DELETE_POOL_MENU_H
