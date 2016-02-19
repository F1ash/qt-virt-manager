#ifndef WIPE_MENU_H
#define WIPE_MENU_H

#include <QMenu>
#include <libvirt/libvirt.h>

class WipeMenu : public QMenu
{
    Q_OBJECT
public:
    explicit WipeMenu(QWidget *parent = nullptr);

signals:
    void             execMethod(const QStringList&);

private:
    QAction         *algorithm_ZERO;
    QAction         *algorithm_NNSA;
    QAction         *algorithm_DOD;
    QAction         *algorithm_BSI;
    QAction         *algorithm_GUTMANN;
    QAction         *algorithm_SCHNEIER;
    QAction         *algorithm_PFITZNER7;
    QAction         *algorithm_PFITZNER33;
    QAction         *algorithm_RANDOM;

private slots:
    void             emitExecMethod(QAction*);
    void             showActionToolTip(QAction*);
};

#endif // WIPE_MENU_H
