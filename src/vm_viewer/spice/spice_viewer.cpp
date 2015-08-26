#include "spice_viewer.h"

Spice_Viewer::Spice_Viewer(
        QWidget      *parent,
        virConnect   *conn,
        QString       arg1,
        QString       arg2) :
    VM_Viewer(parent, conn, arg1, arg2)
{
    TYPE = "SPICE";
    if ( jobConnect!=NULL ) {
        domainPtr = getDomainPtr();
    };
    QString msg;
    if ( domainPtr!=NULL ) {
        // TODO: implement getting URI & Connect to thread
        // get address or hostname from URI
        // driver[+transport]://[username@][hostname][:port]/[path][?extraparameters]
        QString uri(virConnectGetURI(jobConnect));
        addr = uri.split("://").last();
        uri = addr;
        addr = uri.split("/").first();
        uri = addr;
        addr = uri.split(":").first();
        uri = addr;
        if ( uri.contains("@") ) {
            addr = uri.split("@").last();
            uri = addr;
        };
        addr.clear();
        // flag=0 for get running domain xml-description
        runXmlDesc.append( virDomainGetXMLDesc(domainPtr, 0) );
        QDomDocument doc;
        doc.setContent(runXmlDesc);
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
            if ( uri.isEmpty() ) {
                addr = "127.0.0.1";
                uri  = "127.0.0.1";
            };
        };
        if ( addr!=uri && !uri.isEmpty() ) addr = uri;
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

Spice_Viewer::~Spice_Viewer()
{

}

/* public slots */
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

