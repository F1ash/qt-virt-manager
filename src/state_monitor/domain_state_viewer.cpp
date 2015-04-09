#include "domain_state_viewer.h"

#define PERIOD 3

DomainStateViewer::DomainStateViewer(QWidget *parent,
        virConnectPtr conn,
        QString _domainName) :
    QWidget(parent), currWorkConn(conn), domainName(_domainName)
{
    monitorName = new QLabel(domainName, this);
    closeViewer = new QPushButton(
                QIcon::fromTheme("window-close"), "", this);
    cpuLabel = new QLabel("Host CPU\nUsage:", this);
    memLabel = new QLabel("Guest Memory\nUsage:", this);
    cpuGraphic = new QSvgWidget(this);
    memGraphic = new QSvgWidget(this);
    baseLayout = new QGridLayout();
    baseLayout->addWidget(monitorName, 0, 0);
    baseLayout->addWidget(closeViewer, 0, 1, Qt::AlignRight);
    baseLayout->addWidget(cpuLabel, 1, 0);
    baseLayout->addWidget(cpuGraphic, 1, 1);
    baseLayout->addWidget(memLabel, 2, 0);
    baseLayout->addWidget(memGraphic, 2, 1);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(closeViewer, SIGNAL(clicked()),
            this, SLOT(closeDomainStateViewer()));
    // TODO: implement the display of different domain states:
    // CPU Usage, Disk I/O (?), Network I/O (?), MemoryStats.
    domainMonitorThread = new DomainMonitorThread(
                this, currWorkConn, domainName);
    connect(domainMonitorThread, SIGNAL(dataChanged(int, int, int, int)),
            this, SLOT(setData(int, int, int, int)));

    cpuDoc.setContent(HOST_CPU_USAGE_SVG_TEMPLATE);
    cpuPaint = cpuDoc.firstChildElement("svg")
            .firstChildElement("g");
    cpuPercent = cpuDoc.firstChildElement("svg")
            .firstChildElement("text");

    memDoc.setContent(GUEST_MEMORY_STATE_SVG_TEMPLATE);
    memPaint = memDoc.firstChildElement("svg")
            .firstChildElement("g");
    memPercent = memDoc.firstChildElement("svg")
            .firstChildElement("text");
    timerId = startTimer(PERIOD*1000);
}

/* public slots */
void DomainStateViewer::closeDomainStateViewer()
{
    if ( timerId>0 ) {
        killTimer(timerId);
        timerId = 0;
    };
    setEnabled(false);
    domainMonitorThread->deleteLater();
    if ( currWorkConn!=NULL ) {
        virConnectClose(currWorkConn);
        currWorkConn = NULL;
    };
    close();
    emit viewerClosed();
}

/* private slots */
void DomainStateViewer::timerEvent(QTimerEvent *ev)
{
    if ( timerId==ev->timerId() && currWorkConn!=NULL ) {
        if ( !domainMonitorThread->isRunning() )
            domainMonitorThread->start();
    }
}
void DomainStateViewer::setData(
        int _state, int _cpuPercent, int _memPercent, int _mem)
{
    QString state;
    switch (_state) {
    case VIR_DOMAIN_NOSTATE:
        state.append("NOSTATE");
        break;
    case VIR_DOMAIN_RUNNING:
        state.append("RUNNING");
        break;
    case VIR_DOMAIN_BLOCKED:
        state.append("BLOCKED");
        break;
    case VIR_DOMAIN_PAUSED:
        state.append("PAUSED");
        break;
    case VIR_DOMAIN_SHUTDOWN:
        state.append("SHUTDOWN");
        break;
    case VIR_DOMAIN_SHUTOFF:
        state.append("SHUTOFF");
        break;
    case VIR_DOMAIN_CRASHED:
        state.append("CRASHED");
        break;
    case VIR_DOMAIN_PMSUSPENDED:
        state.append("PMSUSPENDED");
        break;
    default:
        state.append("NOSTATE");
        break;
    };
    monitorName->setText(
                QString("State:<br><b>%1</b></br>").arg(state));

    QDomElement _l, line;
    QDomText _data;
    _l = cpuPaint.firstChildElement("line");
    if ( cpuPaint.childNodes().count()>100 ) {
        cpuPaint.removeChild(cpuPaint.childNodes().at(101));
    };
    while ( !_l.isNull() ) {
        int _xCoordinate = _l.attribute("x1").toInt();
        _l.setAttribute("x1", _xCoordinate - 1);
        _l.setAttribute("x2", _xCoordinate - 1);
        _l = _l.nextSiblingElement("line");
    };
    line = cpuDoc.createElement("line");
    line.setAttribute("x1", 100);
    line.setAttribute("y1", 100);
    line.setAttribute("x2", 100);
    line.setAttribute("y2", 100-_cpuPercent);
    cpuPaint.insertBefore(line, cpuPaint.childNodes().at(0));
    cpuPercent.removeChild( cpuPercent.firstChild() );
    _data = cpuDoc.createTextNode(
                QString("%1%").arg(_cpuPercent));
    cpuPercent.appendChild(_data);
    cpuGraphic->load(cpuDoc.toByteArray());

    _l = memPaint.firstChildElement("line");
    if ( memPaint.childNodes().count()>100 ) {
        memPaint.removeChild(memPaint.childNodes().at(101));
    };
    while ( !_l.isNull() ) {
        int _xCoordinate = _l.attribute("x1").toInt();
        _l.setAttribute("x1", _xCoordinate - 1);
        _l.setAttribute("x2", _xCoordinate - 1);
        _l = _l.nextSiblingElement("line");
    };
    line = memDoc.createElement("line");
    line.setAttribute("x1", 100);
    line.setAttribute("y1", 100);
    line.setAttribute("x2", 100);
    line.setAttribute("y2", 100-_memPercent);
    memPaint.insertBefore(line, memPaint.childNodes().at(0));
    memPercent.removeChild( memPercent.firstChild() );
    _data = memDoc.createTextNode(
                QString("%1 KiB").arg(_mem));
    memPercent.appendChild(_data);
    memGraphic->load(memDoc.toByteArray());
}
