#include "proxy_widget.h"
#include <QPainter>
#include <QIcon>
#include <QDebug>

#define ICON_NATIVE_SIZE 256

ProxyWidget::ProxyWidget(QWidget *parent) :
    QWidget(parent)
{
    usedViewMode = HARD_CLASSIC;
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
    setMouseTracking(true);
    connAct = new QAction(this);
    domainsAct = new QAction(this);
    networksAct = new QAction(this);
    storagesAct = new QAction(this);
    secretsAct = new QAction(this);
    ifacesAct = new QAction(this);
    logAct = new QAction(this);
    addAction(connAct);
    addAction(domainsAct);
    addAction(networksAct);
    addAction(storagesAct);
    addAction(secretsAct);
    addAction(ifacesAct);
    addAction(logAct);
    connAct->setShortcut(
                QKeySequence(tr("Ctrl+Alt+C", "Show Connections")));
    domainsAct->setShortcut(
                QKeySequence(tr("Ctrl+Alt+D", "Show VM Machines")));
    networksAct->setShortcut(
                QKeySequence(tr("Ctrl+Alt+N", "Show Networks")));
    storagesAct->setShortcut(
                QKeySequence(tr("Ctrl+Alt+S", "Show Storages")));
    secretsAct->setShortcut(
                QKeySequence(tr("Ctrl+Alt+R", "Show Secrets")));
    ifacesAct->setShortcut(
                QKeySequence(tr("Ctrl+Alt+I", "Show Interfaces")));
    logAct->setShortcut(
                QKeySequence(tr("Ctrl+Alt+L", "Show Log")));
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
    connect(logAct, SIGNAL(triggered(bool)),
            this, SLOT(actionTriggered()));
}

void ProxyWidget::setUsedViewMode(VIEW_MODE _mode)
{
    usedViewMode = _mode;
}

/* public slots */

/* private slots */
void ProxyWidget::actionTriggered()
{
    qDebug()<<"actionTriggered";
    if ( usedViewMode!=SOFT_TOUCHED ) return;
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
    if ( ev->type()==QMouseEvent::MouseButtonRelease ) {
        qDebug()<<"mouseReleaseEvent";
        if        ( r1.contains(ev->pos(), true) ) {
            qDebug()<<"in R1";
        } else if ( r2.contains(ev->pos(), true) ) {
            qDebug()<<"in R2";
        } else if ( r3.contains(ev->pos(), true) ) {
            qDebug()<<"in R3";
        } else if ( r4.contains(ev->pos(), true) ) {
            qDebug()<<"in R4";
        } else if ( r5.contains(ev->pos(), true) ) {
            qDebug()<<"in R5";
        } else if ( r6.contains(ev->pos(), true) ) {
            qDebug()<<"in R6";
        } else if ( r7.contains(ev->pos(), true) ) {
            qDebug()<<"in R7";
        } else if ( r8.contains(ev->pos(), true) ) {
            qDebug()<<"in R8";
        } else if ( r9.contains(ev->pos(), true) ) {
            qDebug()<<"in R9";
        };
    };
}
void ProxyWidget::mouseMoveEvent(QMouseEvent *ev)
{
    ev->accept();
    if ( usedViewMode!=SOFT_TOUCHED ) return;
    if        ( r1.contains(ev->pos(), true) ) {
        if ( !currRect.intersects(r1) ) {
            currRect = r1;
            update();
            qDebug()<<"R1";
        };
    } else if ( r2.contains(ev->pos(), true) ) {
        if ( !currRect.intersects(r2) ) {
            currRect = r2;
            update();
            qDebug()<<"R2";
        };
    } else if ( r3.contains(ev->pos(), true) ) {
        if ( !currRect.intersects(r3) ) {
            currRect = r3;
            update();
            qDebug()<<"R3";
        };
    } else if ( r4.contains(ev->pos(), true) ) {
        if ( !currRect.intersects(r4) ) {
            currRect = r4;
            update();
            qDebug()<<"R4";
        };
    } else if ( r5.contains(ev->pos(), true) ) {
        if ( !currRect.intersects(r5) ) {
            currRect = r5;
            update();
            qDebug()<<"R5";
        };
    } else if ( r6.contains(ev->pos(), true) ) {
        if ( !currRect.intersects(r6) ) {
            currRect = r6;
            update();
            qDebug()<<"R6";
        };
    } else if ( r7.contains(ev->pos(), true) ) {
        if ( !currRect.intersects(r7) ) {
            currRect = r7;
            update();
            qDebug()<<"R7";
        };
    } else if ( r8.contains(ev->pos(), true) ) {
        if ( !currRect.intersects(r8) ) {
            currRect = r8;
            update();
            qDebug()<<"R8";
        };
    } else if ( r9.contains(ev->pos(), true) ) {
        if ( !currRect.intersects(r9) ) {
            currRect = r9;
            update();
            qDebug()<<"R9";
        };
    };
}
void ProxyWidget::paintEvent(QPaintEvent *ev)
{
    ev->accept();

    if ( usedViewMode==SOFT_TOUCHED ) {
        QPainter painter(this);
        if ( commonRect.intersects(ev->rect()) ) {
            painter.setOpacity(0.10);
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
            if ( r1.intersects(currRect) ) {
                rF = r1;
            } else if ( r3.intersects(currRect) ) {
                rF = r3;
            } else if ( r5.intersects(currRect) ) {
                rF = r5;
            } else if ( r6.intersects(currRect) ) {
                rF = r6;
            } else if ( r7.intersects(currRect) ) {
                rF = r7;
            } else if ( r8.intersects(currRect) ) {
                rF = r8;
            } else if ( r9.intersects(currRect) ) {
                rF = r9;
            };
            QRadialGradient gradient(
                        rF.topLeft().x(),
                        rF.topLeft().y(),
                        side/2);
            gradient.setColorAt(
                        1,
                        QColor::fromRgb(173, 255, 47, 25)); // green
            gradient.setColorAt(
                        0,
                        QColor::fromRgb(255, 215, 0, 255)); // gold
            QBrush b(gradient);
            painter.fillRect(rF, b);

            painter.setOpacity(1);
            painter.drawPixmap(
                        r1.adjusted(
                            (r1.width()-side)/2,
                            (r1.height()-side)/2,
                            -(r1.width()-side)/2,
                            -(r1.height()-side)/2),
                        connections.scaled(
                            part,
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation));
            painter.drawPixmap(
                        r3.adjusted(
                            (r3.width()-side)/2,
                            (r3.height()-side)/2,
                            -(r3.width()-side)/2,
                            -(r3.height()-side)/2),
                        domains.scaled(
                            part,
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation));
            painter.drawPixmap(
                        r5.adjusted(
                            (r5.width()-side)/2,
                            (r5.height()-side)/2,
                            -(r5.width()-side)/2,
                            -(r5.height()-side)/2),
                        networks.scaled(
                            part,
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation));
            painter.drawPixmap(
                        r6.adjusted(
                            (r6.width()-side)/2,
                            (r6.height()-side)/2,
                            -(r6.width()-side)/2,
                            -(r6.height()-side)/2),
                        storages.scaled(
                            part,
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation));
            painter.drawPixmap(
                        r7.adjusted(
                            (r7.width()-side)/2,
                            (r7.height()-side)/2,
                            -(r7.width()-side)/2,
                            -(r7.height()-side)/2),
                        secrets.scaled(
                            part,
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation));
            painter.drawPixmap(
                        r8.adjusted(
                            (r8.width()-side)/2,
                            (r8.height()-side)/2,
                            -(r8.width()-side)/2,
                            -(r8.height()-side)/2),
                        ifaces.scaled(
                            part,
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation));
            painter.drawPixmap(
                        r9.adjusted(
                            (r9.width()-side)/2,
                            (r9.height()-side)/2,
                            -(r9.width()-side)/2,
                            -(r9.height()-side)/2),
                        log.scaled(
                            part,
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation));
        };

        QFont _font;
        _font.setBold(true);
        _font.setFamily("Monospace");
        _font.setPointSize(18);
        painter.setFont(_font);
        //painter.setOpacity(0.50);
        painter.setPen(Qt::yellow);
        if ( !r1.intersects(currRect) ) {
            painter.drawText(
                        r1.adjusted(0, heightPart*2/3, 0, 0),
                        Qt::AlignCenter,
                        "Connections\nCtrl+Alt+C");
            qDebug()<<"r1!=ev->rect()"<<r1;
        };
        if ( !r3.intersects(currRect) ) {
            painter.drawText(
                        r3.adjusted(0, heightPart/2, 0, 0),
                        Qt::AlignCenter,
                        "Virtual\nmachines\nCtrl+Alt+D");
            qDebug()<<"r3!=ev->rect()"<<r3;
        };
        if ( !r5.intersects(currRect) ) {
            painter.drawText(
                        r5.adjusted(0, heightPart*2/3, 0, 0),
                        Qt::AlignCenter,
                        "Networks\nCtrl+Alt+N");
            qDebug()<<"r5!=ev->rect()"<<r5;
        };
        if ( !r6.intersects(currRect) ) {
            painter.drawText(
                        r6.adjusted(0, heightPart*2/3, 0, 0),
                        Qt::AlignCenter,
                        "Storages\nCtrl+Alt+S");
            qDebug()<<"r6!=ev->rect()"<<r6;
        };
        if ( !r7.intersects(currRect) ) {
            painter.drawText(
                        r7.adjusted(0, heightPart*2/3, 0, 0),
                        Qt::AlignCenter,
                        "Secrets\nCtrl+Alt+R");
            qDebug()<<"r7!=ev->rect()"<<r7;
        };
        if ( !r8.intersects(currRect) ) {
            painter.drawText(
                        r8.adjusted(0, heightPart*2/3, 0, 0),
                        Qt::AlignCenter,
                        "Interfaces\nCtrl+Alt+I");
            qDebug()<<"r8!=ev->rect()"<<r8;
        };
        if ( !r9.intersects(currRect) ) {
            painter.drawText(
                        r9.adjusted(0, heightPart*2/3, 0, 0),
                        Qt::AlignCenter,
                        "Log\nCtrl+Alt+L");
            qDebug()<<"r9!=ev->rect()"<<r9;
        };
    };
}
