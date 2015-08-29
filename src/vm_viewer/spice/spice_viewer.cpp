#include "spice_viewer.h"

spcHlpThread::spcHlpThread(
        QObject *parent, virConnect *_conn, QString _domain) :
    QThread(parent), currWorkConnection(_conn), domain(_domain)
{

}
void spcHlpThread::run()
{
    if ( NULL!=currWorkConnection )
        domainPtr =virDomainLookupByName(
                    currWorkConnection, domain.toUtf8().data());
    uri.append(virConnectGetURI(currWorkConnection));
    // flag=0 for get running domain xml-description
    runXmlDesc.append( virDomainGetXMLDesc(domainPtr, 0) );
}

Spice_Viewer::Spice_Viewer(
        QWidget      *parent,
        virConnect   *conn,
        QString       arg1,
        QString       arg2) :
    VM_Viewer(parent, conn, arg1, arg2)
{
    TYPE = "SPICE";
    hlpThread = new spcHlpThread(this, jobConnect, domain);
    connect(hlpThread, SIGNAL(finished()),
            this, SLOT(init()));
    hlpThread->start();
}

/* public slots */
void Spice_Viewer::init()
{
    // get address or hostname from URI
    // driver[+transport]://[username@][hostname][:port]/[path][?extraparameters]
    QString msg;
    if ( hlpThread->domainPtr!=NULL ) {
        addr = hlpThread->uri.split("://").last();
        hlpThread->uri = addr;
        addr = hlpThread->uri.split("/").first();
        hlpThread->uri = addr;
        addr = hlpThread->uri.split(":").first();
        hlpThread->uri = addr;
        if ( hlpThread->uri.contains("@") ) {
            addr = hlpThread->uri.split("@").last();
            hlpThread->uri = addr;
        };
        addr.clear();
        QDomDocument doc;
        doc.setContent(hlpThread->runXmlDesc);
        QDomElement graph = doc.firstChildElement("domain")
           .firstChildElement("devices")
           .firstChildElement("graphics");
        //qDebug()<<doc.toByteArray(4);
        if (graph.hasAttribute("listen")) {
            // for listen address
            addr = graph.attribute("listen");
        } else if ( !graph.firstChildElement("listen").isNull() ) {
            // for listen address from virt.network
            addr = graph.firstChildElement("listen")
                    .attribute("address");
        } else {
            if ( hlpThread->uri.isEmpty() ) {
                addr = "127.0.0.1";
                hlpThread->uri  = "127.0.0.1";
            };
        };
        if ( addr!=hlpThread->uri && !hlpThread->uri.isEmpty() )
            addr = hlpThread->uri;
        port = (graph.hasAttribute("port"))?
                    graph.attribute("port").toInt() : 5900;
        //qDebug()<<"address:"<<addr<<port;
        if ( !graph.isNull() && graph.attribute("type")=="spice" ) {
            spiceWdg = new QSpiceWidget(this);
            spiceWdg->setWidgetResizable(true);
            setCentralWidget(spiceWdg);
            // use toolbar
            viewerToolBar->setVisible(true);
            actFullScreen = new QShortcut(QKeySequence(tr("Shift+F11", "View|Full Screen")), this);
            connect(actFullScreen, SIGNAL(activated()), SLOT(FullScreenTriggered()));
            connect(spiceWdg, SIGNAL(DisplayResize(QSize)), SLOT(DisplayResize(QSize)));
            spiceWdg->Connect(QString("spice://%1:%2").arg(addr).arg(port));
        } else {
            msg = QString("In '<b>%1</b>':<br> Unsupported type '%2'.<br> Use external Viewer.")
                    .arg(domain).arg((!graph.isNull())? graph.attribute("type"):"???");
            sendErrMsg(msg);
            showErrorInfo(msg);
            startCloseProcess();
        };
    } else {
        msg = QString("In '<b>%1</b>':<br> Connection or Domain is NULL...")
                .arg(domain);
        sendErrMsg(msg);
        showErrorInfo(msg);
        startCloseProcess();
    };
    sendConnErrors();
    //qDebug()<<msg<<"viewer inits";
}
void Spice_Viewer::reconnectToDomain()
{
    QSpiceWidget *wdg = static_cast<QSpiceWidget*>(centralWidget());
    if ( NULL!=wdg ) {
        wdg->Disconnect();
        delete wdg;
        wdg = NULL;
        spiceWdg = new QSpiceWidget(this);
        spiceWdg->setWidgetResizable(true);
        setCentralWidget(spiceWdg);
        spiceWdg->Connect(QString("spice://%1:%2").arg(addr).arg(port));
    };
}
void Spice_Viewer::sendKeySeqToDomain(Qt::Key key)
{
    spiceWdg->SendKeySequience(key);
}

/* private slots */
void Spice_Viewer::timerEvent(QTimerEvent *ev)
{
    if ( ev->timerId()==killTimerId ) {
        counter++;
        closeProcess->setValue(counter*PERIOD*6);
        if ( TIMEOUT<counter*PERIOD*6 ) {
            killTimer(killTimerId);
            killTimerId = 0;
            counter = 0;
            close();
        };
    }
}

void Spice_Viewer::DisplayResize(const QSize &size)
{
    resize(size);
}

void Spice_Viewer::FullScreenTriggered()
{
    if (isFullScreen())
        setWindowState(Qt::WindowNoState);
    else
        setWindowState(Qt::WindowFullScreen);
}

