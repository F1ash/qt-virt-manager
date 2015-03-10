#include "spice_viewer.h"

Spice_Viewer::Spice_Viewer(
        QWidget      *parent,
        virConnect   *conn,
        QString       arg1,
        QString       arg2) :
    VM_Viewer(parent, conn, arg1, arg2)
{
    if ( jobConnect!=NULL ) {
        domainPtr = getDomainPtr();
    };
    QString msg;
    if ( domainPtr!=NULL ) {
        runXmlDesc.append( virDomainGetXMLDesc(domainPtr, 0) );
        QDomDocument doc;
        doc.setContent(runXmlDesc);
        QDomElement graph = doc.firstChildElement("domain")
           .firstChildElement("devices")
           .firstChildElement("graphics");
        if ( !graph.isNull() && graph.attribute("type")=="spice" ) {
            // use toolbar
            viewerToolBar->setEnabled(true);
        } else {
            msg = QString("In '<b>%1</b>': Unsupported type '%2'.<br> Use external Viewer.")
                    .arg(domain).arg((!graph.isNull())? graph.attribute("type"):"???");
            sendErrMsg(msg);
            setCentralWidget(new QLabel(msg, this));
            startCloseProcess();
        };
    } else {
        msg = QString("In '<b>%1</b>': Connect or Domain is NULL...")
                .arg(domain);
        sendErrMsg(msg);
        setCentralWidget(new QLabel(msg, this));
        startCloseProcess();
    };
    sendConnErrors();
    //qDebug()<<msg<<"viewer inits";
}

Spice_Viewer::~Spice_Viewer()
{

}

/* public slots */

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

