#ifndef DOMAIN_STATE_VIEWER_H
#define DOMAIN_STATE_VIEWER_H

#include <QWidget>
#include <QtSvg/QSvgWidget>
#include <QtXml/QDomDocument>
#include <QTimerEvent>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include "domain_monitor_thread.h"
#include "svg_templates.h"

class DomainStateViewer : public QWidget
{
    Q_OBJECT
public:
    explicit DomainStateViewer(
            QWidget        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr,
            QString         _domainName = QString());
    const QString        domainName;

signals:
    void                 viewerClosed();
    void                 errorMsg(QString&);

private:
    virConnectPtr*       ptr_ConnPtr;
    DomainMonitorThread
                        *domainMonitorThread;
    int                  timerId = 0;

    QLabel              *monitorName, *cpuLabel,
                        *memLabel;
    QSvgWidget          *cpuGraphic, *memGraphic;
    QPushButton         *closeViewer;
    QGridLayout         *baseLayout;
    QWidget             *baseWdg;
    QVBoxLayout         *commonLayout;

    QDomDocument         cpuDoc, memDoc;
    QDomElement          cpuPaint, cpuPercent,
                         memPaint, memPercent;

public slots:
    void                 closeDomainStateViewer();

private slots:
    void                 timerEvent(QTimerEvent*);
    void                 setData(int, int, int, int);
};

#endif // DOMAIN_STATE_VIEWER_H
