#ifndef DOMAIN_STATE_MONITOR_H
#define DOMAIN_STATE_MONITOR_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QSettings>
#include <QComboBox>
#include <QCloseEvent>
#include <QVBoxLayout>
#include "domain_state_viewer.h"

class DomainStateMonitor : public QMainWindow
{
    Q_OBJECT
public:
    explicit DomainStateMonitor(QWidget *parent = Q_NULLPTR);

signals:
    void             visibilityChanged(bool);

private:
    QSettings        settings;
    QComboBox       *monitoredDomainList;
    QStackedWidget  *monitoredDomains;
    QVBoxLayout     *monitorLayout;
    QWidget         *monitorWdg;

public slots:
    void             setNewMonitoredDomain(virConnectPtr*, const QString&, const QString&);
    void             changeVisibility();
    void             saveCurrentState();
    void             stopMonitoring();

private slots:
    void             closeEvent(QCloseEvent*);
    void             removeClosedViewer();
};

#endif // DOMAIN_STATE_MONITOR_H
