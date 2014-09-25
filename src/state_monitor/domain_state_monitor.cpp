#include "domain_state_monitor.h"

DomainStateMonitor::DomainStateMonitor(QWidget *parent) :
    QMainWindow(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    setMinimumSize(100, 100);
    setContentsMargins(0, 0, 0, 5);
    setWindowTitle("Domain State Monitor");
    settings.beginGroup("DomainStateMonitor");
    restoreState(settings.value("State").toByteArray());
    restoreGeometry(settings.value("Geometry").toByteArray());
    setVisible(!settings.value("Visible", false).toBool());
    settings.endGroup();
    monitoredDomainList = new QComboBox(this);
    monitoredDomains = new QStackedWidget(this);
    monitorLayout = new QVBoxLayout();
    monitorLayout->addWidget(monitoredDomainList);
    monitorLayout->addWidget(monitoredDomains);
    monitorWdg = new QWidget(this);
    monitorWdg->setLayout(monitorLayout);
    setCentralWidget(monitorWdg);
    connect(monitoredDomainList, SIGNAL(currentIndexChanged(int)),
            monitoredDomains, SLOT(setCurrentIndex(int)));
}

/* public slots */
void DomainStateMonitor::setNewMonitoredDomain(
        virConnectPtr conn,
        QString &connName,
        QString &domainName)
{
    QString _id = QString("%1 || %2")
            .arg(domainName)
            .arg(connName);
    int idx =
            monitoredDomainList->findText(
                _id,
                Qt::MatchExactly | Qt::MatchCaseSensitive);
    if ( idx<0 ) {
        monitoredDomainList->addItem(_id);
        int i = monitoredDomains->addWidget(
                    new DomainStateViewer(
                        this, conn, domainName));
        connect(monitoredDomains->widget(i), SIGNAL(viewerClosed()),
                this, SLOT(removeClosedViewer()));
        qDebug()<<domainName<<"add to StateMonitor";
    };
}
void DomainStateMonitor::changeVisibility()
{
    setVisible(!isVisible());
}
void DomainStateMonitor::saveCurrentState()
{
    settings.beginGroup("DomainStateMonitor");
    settings.setValue("State", saveState());
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("Visible", isVisible());
    settings.endGroup();
    settings.sync();
}

/* private slots */
void DomainStateMonitor::closeEvent(QCloseEvent *ev)
{
    ev->ignore();
    changeVisibility();
}
void DomainStateMonitor::removeClosedViewer()
{
    DomainStateViewer *wdg = static_cast<DomainStateViewer*>(sender());
    int i = monitoredDomains->indexOf(wdg);
    disconnect(monitoredDomains->widget(i), SIGNAL(viewerClosed()),
               this, SLOT(removeClosedViewer()));
    monitoredDomains->removeWidget(wdg);
    monitoredDomainList->removeItem(i);
    qDebug()<<wdg->domainName<<"remove from StateMonitor";
    delete wdg;
    wdg = NULL;
}
