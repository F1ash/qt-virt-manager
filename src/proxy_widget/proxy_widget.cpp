#include "proxy_widget.h"
#include <QPainter>
#include <QDebug>

#define ICON_NATIVE_SIZE 256

ProxyWidget::ProxyWidget(QWidget *parent) :
    QWidget(parent)
{
    usedViewMode = HARD_CLASSIC;
    isTriggered = false;
    emblem = QIcon::fromTheme("virtual-engineering")
            .pixmap(ICON_NATIVE_SIZE);
    connections = QIcon::fromTheme("connections")
            .pixmap(ICON_NATIVE_SIZE);
    log = QIcon::fromTheme("utilities-log-viewer")
            .pixmap(ICON_NATIVE_SIZE);
    domains = QIcon::fromTheme("vm-machines")
            .pixmap(ICON_NATIVE_SIZE);
    networks = QIcon::fromTheme("networks")
            .pixmap(ICON_NATIVE_SIZE);
    storages = QIcon::fromTheme("storages")
            .pixmap(ICON_NATIVE_SIZE);
    secrets = QIcon::fromTheme("security")
            .pixmap(ICON_NATIVE_SIZE);
    ifaces = QIcon::fromTheme("interfaces")
            .pixmap(ICON_NATIVE_SIZE);
    nwfilters = QIcon::fromTheme("nwfilter")
            .pixmap(ICON_NATIVE_SIZE);
    setMouseTracking(true);
    connAct = new QAction(this);
    domainsAct = new QAction(this);
    networksAct = new QAction(this);
    storagesAct = new QAction(this);
    secretsAct = new QAction(this);
    ifacesAct = new QAction(this);
    nfAct = new QAction(this);
    logAct = new QAction(this);
    next = new QAction(this);
    prev = new QAction(this);
    addAction(connAct);
    addAction(domainsAct);
    addAction(networksAct);
    addAction(storagesAct);
    addAction(secretsAct);
    addAction(ifacesAct);
    addAction(nfAct);
    addAction(logAct);
    addAction(next);
    addAction(prev);
    connAct->setShortcut(
                QKeySequence(tr("Ctrl+Alt+C", "Show Connections")));
    domainsAct->setShortcut(
                QKeySequence(tr("Ctrl+Alt+D", "Show VM Machines")));
    networksAct->setShortcut(
                QKeySequence(tr("Ctrl+Alt+N", "Show Networks")));
    storagesAct->setShortcut(
                QKeySequence(tr("Ctrl+Alt+S", "Show Storages")));
    secretsAct->setShortcut(
                QKeySequence(tr("Ctrl+Alt+E", "Show Secrets")));
    ifacesAct->setShortcut(
                QKeySequence(tr("Ctrl+Alt+I", "Show Interfaces")));
    nfAct->setShortcut(
                QKeySequence(tr("Ctrl+Alt+F", "Show Network Filters")));
    logAct->setShortcut(
                QKeySequence(tr("Ctrl+Alt+G", "Show Log")));
    next->setShortcut(
                QKeySequence(tr("Ctrl+Alt+Right", "Next Dock")));
    prev->setShortcut(
                QKeySequence(tr("Ctrl+Alt+Left", "Prev Dock")));
    connect(connAct, SIGNAL(triggered(bool)),
            this, SLOT(actionTriggered()));
    connect(domainsAct, SIGNAL(triggered(bool)),
            this, SLOT(actionTriggered()));
    connect(networksAct, SIGNAL(triggered(bool)),
            this, SLOT(actionTriggered()));
    connect(storagesAct, SIGNAL(triggered(bool)),
            this, SLOT(actionTriggered()));
    connect(secretsAct, SIGNAL(triggered(bool)),
            this, SLOT(actionTriggered()));
    connect(ifacesAct, SIGNAL(triggered(bool)),
            this, SLOT(actionTriggered()));
    connect(nfAct, SIGNAL(triggered(bool)),
            this, SLOT(actionTriggered()));
    connect(logAct, SIGNAL(triggered(bool)),
            this, SLOT(actionTriggered()));
    connect(next, SIGNAL(triggered(bool)),
            this, SLOT(actionTriggered()));
    connect(prev, SIGNAL(triggered(bool)),
            this, SLOT(actionTriggered()));
    setContentsMargins(0, 0, 0, 0);
}

void ProxyWidget::setUsedViewMode(VIEW_MODE _mode)
{
    usedViewMode = _mode;
    isTriggered = false;
}
void ProxyWidget::returnToUntriggered()
{
    isTriggered = false;
    update();
}

/* private slots */
void ProxyWidget::actionTriggered()
{
    if ( sender()==next ) {
        if ( isTriggered ) emit viewNextDock();
        return;
    } else if ( sender()==prev ) {
        if ( isTriggered ) emit viewPrevDock();
        return;
    };
    isTriggered = true;
    if ( usedViewMode!=SOFT_TOUCHED ) return;
    // see for:
    // MainWindow::initConnListWidget() &
    // MainWindow::initDockWidgets()
    // sets the object names for docks.
    if        ( sender()==connAct ) {
        emit viewDock("connListDock");
    } else if ( sender()==domainsAct ) {
        emit viewDock("domainDock");
    } else if ( sender()==networksAct ) {
        emit viewDock("networkDock");
    } else if ( sender()==storagesAct ) {
        emit viewDock("storagePoolDock");
    } else if ( sender()==ifacesAct ) {
        emit viewDock("ifaceDock");
    } else if ( sender()==secretsAct ) {
        emit viewDock("secretDock");
    } else if ( sender()==nfAct ) {
        emit viewDock("nwfilterDock");
    } else if ( sender()==logAct ) {
        emit viewDock("logDock");
    };
    update();
}
void ProxyWidget::resizeEvent(QResizeEvent *ev)
{
    ev->accept();
    widthPart = frameSize().width()/3;
    heightPart = frameSize().height()/3;
    side = qMin(widthPart, heightPart);
    /* Rectangles order
     * 1 2 3
     * 4 5 6
     * 7 8 9
     */
    part = QSize(side, side);
    r1 = QRect(0, 0, widthPart, heightPart);
    r2 = QRect(widthPart, 0, widthPart, heightPart);
    r3 = QRect(widthPart*2, 0, widthPart, heightPart);
    r4 = QRect(0, heightPart, widthPart, heightPart);
    r5 = QRect(widthPart, heightPart, widthPart, heightPart);
    r6 = QRect(widthPart*2, heightPart, widthPart, heightPart);
    r7 = QRect(0, heightPart*2, widthPart, heightPart);
    r8 = QRect(widthPart, heightPart*2, widthPart, heightPart);
    r9 = QRect(widthPart*2, heightPart*2, widthPart, heightPart);
    commonRect = QRect(QPoint(0, 0), frameSize());
    update();
}
void ProxyWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    ev->accept();
    if ( usedViewMode!=SOFT_TOUCHED ) return;
    if ( isTriggered ) return;
    if ( ev->type()==QMouseEvent::MouseButtonRelease ) {
        if        ( r1.contains(ev->pos(), true) ) {
            //qDebug()<<"in R1";
            connAct->trigger();
        } else if ( r2.contains(ev->pos(), true) ) {
            //qDebug()<<"in R2";
        } else if ( r3.contains(ev->pos(), true) ) {
            //qDebug()<<"in R3";
            domainsAct->trigger();
        } else if ( r4.contains(ev->pos(), true) ) {
            //qDebug()<<"in R4";
            nfAct->trigger();
        } else if ( r5.contains(ev->pos(), true) ) {
            //qDebug()<<"in R5";
            networksAct->trigger();
        } else if ( r6.contains(ev->pos(), true) ) {
            //qDebug()<<"in R6";
            storagesAct->trigger();
        } else if ( r7.contains(ev->pos(), true) ) {
            //qDebug()<<"in R7";
            secretsAct->trigger();
        } else if ( r8.contains(ev->pos(), true) ) {
            //qDebug()<<"in R8";
            ifacesAct->trigger();
        } else if ( r9.contains(ev->pos(), true) ) {
            //qDebug()<<"in R9";
            logAct->trigger();
        };
    };
}
void ProxyWidget::mouseMoveEvent(QMouseEvent *ev)
{
    ev->accept();
    if ( usedViewMode!=SOFT_TOUCHED ) return;
    if ( isTriggered ) return;
    if        ( r1.contains(ev->pos(), true) ) {
        if ( !currRect.intersects(r1) ) {
            currRect = r1;
            update();
            //qDebug()<<"R1";
        };
    } else if ( r2.contains(ev->pos(), true) ) {
        if ( !currRect.intersects(r2) ) {
            currRect = r2;
            update();
            //qDebug()<<"R2";
        };
    } else if ( r3.contains(ev->pos(), true) ) {
        if ( !currRect.intersects(r3) ) {
            currRect = r3;
            update();
            //qDebug()<<"R3";
        };
    } else if ( r4.contains(ev->pos(), true) ) {
        if ( !currRect.intersects(r4) ) {
            currRect = r4;
            update();
            //qDebug()<<"R4";
        };
    } else if ( r5.contains(ev->pos(), true) ) {
        if ( !currRect.intersects(r5) ) {
            currRect = r5;
            update();
            //qDebug()<<"R5";
        };
    } else if ( r6.contains(ev->pos(), true) ) {
        if ( !currRect.intersects(r6) ) {
            currRect = r6;
            update();
            //qDebug()<<"R6";
        };
    } else if ( r7.contains(ev->pos(), true) ) {
        if ( !currRect.intersects(r7) ) {
            currRect = r7;
            update();
            //qDebug()<<"R7";
        };
    } else if ( r8.contains(ev->pos(), true) ) {
        if ( !currRect.intersects(r8) ) {
            currRect = r8;
            update();
            //qDebug()<<"R8";
        };
    } else if ( r9.contains(ev->pos(), true) ) {
        if ( !currRect.intersects(r9) ) {
            currRect = r9;
            update();
            //qDebug()<<"R9";
        };
    };
}
void ProxyWidget::paintEvent(QPaintEvent *ev)
{
    ev->accept();
    if ( isTriggered ) {
        QPainter painter(this);
        painter.eraseRect(commonRect);
        return;
    };

    if ( usedViewMode==SOFT_TOUCHED ) {
        QPainter painter(this);
        if ( commonRect.intersects(ev->rect()) ) {
            painter.setOpacity(0.20);
            painter.drawPixmap(
                        commonRect.adjusted(
                            (commonRect.width()-4*side)/2,
                            (commonRect.height()-4*side)/2,
                            -(commonRect.width()-4*side)/2,
                            -(commonRect.height()-4*side)/2),
                        emblem.scaled(
                            commonRect.size(),
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation));

            painter.setOpacity(1);
            QRect rF;
            QPoint dR1, dR3, dR4, dR5, dR6, dR7, dR8, dR9;
            if ( r1.intersects(currRect) ) {
                rF = r1; dR1 = QPoint(5, 5);
            } else if ( r3.intersects(currRect) ) {
                rF = r3; dR3 = QPoint(5, 5);
            } else if ( r4.intersects(currRect) ) {
                rF = r4; dR4 = QPoint(5, 5);
            } else if ( r5.intersects(currRect) ) {
                rF = r5; dR5 = QPoint(5, 5);
            } else if ( r6.intersects(currRect) ) {
                rF = r6; dR6 = QPoint(5, 5);
            } else if ( r7.intersects(currRect) ) {
                rF = r7; dR7 = QPoint(5, 5);
            } else if ( r8.intersects(currRect) ) {
                rF = r8; dR8 = QPoint(5, 5);
            } else if ( r9.intersects(currRect) ) {
                rF = r9; dR9 = QPoint(5, 5);
            };
            QRadialGradient gradient(
                        rF.center().x(),
                        rF.center().y()+side/3,
                        side/2);
            gradient.setColorAt(
                        1,
                        QColor::fromRgb(173, 255, 47, 25)); // green
            gradient.setColorAt(
                        0,
                        QColor::fromRgb(255, 215, 0, 255)); // gold
            QBrush b(gradient);
            painter.setBrush(b);
            painter.setPen(QPen(QBrush(), 0));
            QPoint p = QPoint(
                        rF.center().x(),
                        rF.center().y()+side/4);
            rF.moveCenter(p);
            painter.drawEllipse(rF);

            painter.setOpacity(1);
            painter.drawPixmap(
                        r1.adjusted(
                            (r1.width()-side)/2+dR1.x(),
                            (r1.height()-side)/2+dR1.y(),
                            -(r1.width()-side)/2+dR1.x(),
                            -(r1.height()-side)/2+dR1.y()),
                        connections.scaled(
                            part,
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation));
            painter.drawPixmap(
                        r3.adjusted(
                            (r3.width()-side)/2+dR3.x(),
                            (r3.height()-side)/2+dR3.y(),
                            -(r3.width()-side)/2+dR3.x(),
                            -(r3.height()-side)/2+dR3.y()),
                        domains.scaled(
                            part,
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation));
            painter.drawPixmap(
                        r4.adjusted(
                            (r4.width()-side)/2+dR4.x(),
                            (r4.height()-side)/2+dR4.y(),
                            -(r4.width()-side)/2+dR4.x(),
                            -(r4.height()-side)/2+dR4.y()),
                        nwfilters.scaled(
                            part,
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation));
            painter.drawPixmap(
                        r5.adjusted(
                            (r5.width()-side)/2+dR5.x(),
                            (r5.height()-side)/2+dR5.y(),
                            -(r5.width()-side)/2+dR5.x(),
                            -(r5.height()-side)/2+dR5.y()),
                        networks.scaled(
                            part,
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation));
            painter.drawPixmap(
                        r6.adjusted(
                            (r6.width()-side)/2+dR6.x(),
                            (r6.height()-side)/2+dR6.y(),
                            -(r6.width()-side)/2+dR6.x(),
                            -(r6.height()-side)/2+dR6.y()),
                        storages.scaled(
                            part,
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation));
            painter.drawPixmap(
                        r7.adjusted(
                            (r7.width()-side)/2+dR7.x(),
                            (r7.height()-side)/2+dR7.y(),
                            -(r7.width()-side)/2+dR7.x(),
                            -(r7.height()-side)/2+dR7.y()),
                        secrets.scaled(
                            part,
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation));
            painter.drawPixmap(
                        r8.adjusted(
                            (r8.width()-side)/2+dR8.x(),
                            (r8.height()-side)/2+dR8.y(),
                            -(r8.width()-side)/2+dR8.x(),
                            -(r8.height()-side)/2+dR8.y()),
                        ifaces.scaled(
                            part,
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation));
            painter.drawPixmap(
                        r9.adjusted(
                            (r9.width()-side)/2+dR9.x(),
                            (r9.height()-side)/2+dR9.y(),
                            -(r9.width()-side)/2+dR9.x(),
                            -(r9.height()-side)/2+dR9.y()),
                        log.scaled(
                            part,
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation));
        };

        QFont _font;
        _font.setBold(true);
        _font.setFamily("Monospace");
        _font.setPixelSize(heightPart/8);
        painter.setFont(_font);
        //painter.setOpacity(0.50);
        if ( !r1.intersects(currRect) ) {
            painter.setPen(QColor::fromRgb(248, 248, 255, 255)); // ghostwhite
            painter.drawText(
                        r1.adjusted(0, heightPart*2/3, 0, 0),
                        Qt::AlignCenter,
                        "Connections\nCtrl+Alt+C");
            //qDebug()<<"r1!=ev->rect()"<<r1;
        } else {
            painter.setPen(Qt::yellow);
            painter.drawText(
                        r1.adjusted(0, heightPart*2/3, 0, 0),
                        Qt::AlignCenter,
                        "Connections\nCtrl+Alt+C");
        };
        if ( !r3.intersects(currRect) ) {
            painter.setPen(QColor::fromRgb(248, 248, 255, 255)); // ghostwhite
            painter.drawText(
                        r3.adjusted(0, heightPart/2, 0, 0),
                        Qt::AlignCenter,
                        "Virtual\nmachines\nCtrl+Alt+D");
            //qDebug()<<"r3!=ev->rect()"<<r3;
        } else {
            painter.setPen(Qt::yellow);
            painter.drawText(
                        r3.adjusted(0, heightPart/2, 0, 0),
                        Qt::AlignCenter,
                        "Virtual\nmachines\nCtrl+Alt+D");
        };
        if ( !r4.intersects(currRect) ) {
            painter.setPen(QColor::fromRgb(248, 248, 255, 255)); // ghostwhite
            painter.drawText(
                        r4.adjusted(0, heightPart/2, 0, 0),
                        Qt::AlignCenter,
                        "Network\nfilters\nCtrl+Alt+F");
            //qDebug()<<"r4!=ev->rect()"<<r4;
        } else {
            painter.setPen(Qt::yellow);
            painter.drawText(
                        r4.adjusted(0, heightPart/2, 0, 0),
                        Qt::AlignCenter,
                        "Network\nfilters\nCtrl+Alt+F");
        };
        if ( !r5.intersects(currRect) ) {
            painter.setPen(QColor::fromRgb(248, 248, 255, 255)); // ghostwhite
            painter.drawText(
                        r5.adjusted(0, heightPart*2/3, 0, 0),
                        Qt::AlignCenter,
                        "Networks\nCtrl+Alt+N");
            //qDebug()<<"r5!=ev->rect()"<<r5;
        } else {
            painter.setPen(Qt::yellow);
            painter.drawText(
                        r5.adjusted(0, heightPart*2/3, 0, 0),
                        Qt::AlignCenter,
                        "Networks\nCtrl+Alt+N");
        };
        if ( !r6.intersects(currRect) ) {
            painter.setPen(QColor::fromRgb(248, 248, 255, 255)); // ghostwhite
            painter.drawText(
                        r6.adjusted(0, heightPart*2/3, 0, 0),
                        Qt::AlignCenter,
                        "Storages\nCtrl+Alt+S");
            //qDebug()<<"r6!=ev->rect()"<<r6;
        } else {
            painter.setPen(Qt::yellow);
            painter.drawText(
                        r6.adjusted(0, heightPart*2/3, 0, 0),
                        Qt::AlignCenter,
                        "Storages\nCtrl+Alt+S");
        };
        if ( !r7.intersects(currRect) ) {
            painter.setPen(QColor::fromRgb(248, 248, 255, 255)); // ghostwhite
            painter.drawText(
                        r7.adjusted(0, heightPart*2/3, 0, 0),
                        Qt::AlignCenter,
                        "Secrets\nCtrl+Alt+E");
            //qDebug()<<"r7!=ev->rect()"<<r7;
        } else {
            painter.setPen(Qt::yellow);
            painter.drawText(
                        r7.adjusted(0, heightPart*2/3, 0, 0),
                        Qt::AlignCenter,
                        "Secrets\nCtrl+Alt+E");
        };
        if ( !r8.intersects(currRect) ) {
            painter.setPen(QColor::fromRgb(248, 248, 255, 255)); // ghostwhite
            painter.drawText(
                        r8.adjusted(0, heightPart*2/3, 0, 0),
                        Qt::AlignCenter,
                        "Interfaces\nCtrl+Alt+I");
            //qDebug()<<"r8!=ev->rect()"<<r8;
        } else {
            painter.setPen(Qt::yellow);
            painter.drawText(
                        r8.adjusted(0, heightPart*2/3, 0, 0),
                        Qt::AlignCenter,
                        "Interfaces\nCtrl+Alt+I");
        };
        if ( !r9.intersects(currRect) ) {
            painter.setPen(QColor::fromRgb(248, 248, 255, 255)); // ghostwhite
            painter.drawText(
                        r9.adjusted(0, heightPart*2/3, 0, 0),
                        Qt::AlignCenter,
                        "Log\nCtrl+Alt+G");
            //qDebug()<<"r9!=ev->rect()"<<r9;
        } else {
            painter.setPen(Qt::yellow);
            painter.drawText(
                        r9.adjusted(0, heightPart*2/3, 0, 0),
                        Qt::AlignCenter,
                        "Log\nCtrl+Alt+G");
        };
    };
}
