#ifndef DOMAIN_STATE_VIEWER_H
#define DOMAIN_STATE_VIEWER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include "domain_monitor_thread.h"

class DomainStateViewer : public QWidget
{
    Q_OBJECT
public:
    explicit DomainStateViewer(
            QWidget *parent = 0,
            virConnectPtr conn = NULL,
            QString _domainName = QString());
    const QString        domainName;

signals:
    void viewerClosed();

private:
    virConnectPtr        currWorkConn;
    DomainMonitorThread
                        *domainMonitorThread;

    QLabel              *monitorName;
    QPushButton         *closeViewer;
    QGridLayout         *baseLayout;
    QWidget             *baseWdg;
    QVBoxLayout         *commonLayout;

public slots:

private slots:
    void closeDomainStateViewer();
};

#endif // DOMAIN_STATE_VIEWER_H
