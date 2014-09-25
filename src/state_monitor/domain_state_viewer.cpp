#include "domain_state_viewer.h"

DomainStateViewer::DomainStateViewer(QWidget *parent,
        virConnectPtr conn,
        QString _domainName) :
    QWidget(parent), currWorkConn(conn), domainName(_domainName)
{
    monitorName = new QLabel(domainName, this);
    closeViewer = new QPushButton(
                QIcon::fromTheme("window-close"), "", this);
    baseLayout = new QGridLayout();
    baseLayout->addWidget(monitorName, 0, 0);
    baseLayout->addWidget(closeViewer, 0, 1, Qt::AlignRight);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(closeViewer, SIGNAL(clicked()),
            this, SLOT(closeDomainStateViewer()));
    // TODO: implement the display of different domain states:
    // CPU Usage, Disk I/O, Network I/O, MemoryStats.
    domainMonitorThread = new DomainMonitorThread(
                this, currWorkConn, domainName);
}

/* public slots */

/* private slots */
void DomainStateViewer::closeDomainStateViewer()
{
    setEnabled(false);
    close();
    emit viewerClosed();
}
