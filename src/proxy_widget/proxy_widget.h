#ifndef PROXY_WIDGET_H
#define PROXY_WIDGET_H

#include <QWidget>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPixmap>
#include <QAction>

enum VIEW_MODE {
    HARD_CLASSIC = 0,
    SOFT_TOUCHED
};

class ProxyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProxyWidget(QWidget *parent = nullptr);
    void        setUsedViewMode(VIEW_MODE);
    void        returnToUntriggered();

signals:
    void        viewDock(const QString&);
    void        viewNextDock();
    void        viewPrevDock();

private:
    VIEW_MODE   usedViewMode;
    bool        isTriggered;
    QPixmap     emblem;
    QPixmap     connections;
    QPixmap     log;
    QPixmap     domains;
    QPixmap     networks;
    QPixmap     storages;
    QPixmap     secrets;
    QPixmap     ifaces;
    QPixmap     nwfilters;
    QRect       r1, r2, r3, r4,
                r5, r6, r7, r8,
                r9, commonRect, currRect;
    QSize       part;
    QAction    *connAct, *domainsAct,
               *networksAct, *storagesAct,
               *secretsAct, *ifacesAct, *nfAct,
               *logAct, *next, *prev;
    int         widthPart, heightPart, side;

private slots:
    void        actionTriggered();
    void        resizeEvent(QResizeEvent*);
    void        mouseReleaseEvent(QMouseEvent*);
    void        mouseMoveEvent(QMouseEvent*);
    void        paintEvent(QPaintEvent*);
};

#endif // PROXY_WIDGET_H
