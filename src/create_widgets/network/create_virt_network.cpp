#include "create_virt_network.h"

NetHelperThread::NetHelperThread(QObject *parent, virConnectPtr *connPtrPtr) :
    _VirtThread(parent, connPtrPtr)
{

}
void NetHelperThread::run()
{
    if ( nullptr==ptr_ConnPtr || nullptr==*ptr_ConnPtr ) {
        emit ptrIsNull();
        return;
    };
    if ( virConnectRef(*ptr_ConnPtr)<0 ) {
        sendConnErrors();
        return;
    };
    // something data reading
    if ( virConnectClose(*ptr_ConnPtr)<0 )
        sendConnErrors();
}

CreateVirtNetwork::CreateVirtNetwork(
        QWidget *parent, TASK _task) :
    QMainWindow(parent), task(_task)
{
    setAttribute(Qt::WA_DeleteOnClose);
    xmlFileName = task.args.path;
    ptr_ConnPtr = task.srcConnPtr;
    setWindowTitle("Network Editor");
    setWindowIcon(QIcon::fromTheme("virtual-engineering"));
    settings.beginGroup("VirtNetControl");
    restoreGeometry(settings.value("NetCreateGeometry").toByteArray());
    settings.endGroup();
    xml = new QTemporaryFile(this);
    xml->setAutoRemove(false);
    xml->setFileTemplate(
                QString("%1%2XML_Desc-XXXXXX.xml")
                .arg(QDir::tempPath())
                .arg(QDir::separator()));
    setEnabled(false);
    helperThread = new NetHelperThread(this, ptr_ConnPtr);
    connect(helperThread, SIGNAL(finished()),
            this, SLOT(readCapabilities()));
    connect(helperThread, SIGNAL(errorMsg(QString&,uint)),
            this, SIGNAL(errorMsg(QString&)));
    helperThread->start();

}
CreateVirtNetwork::~CreateVirtNetwork()
{
    settings.beginGroup("VirtNetControl");
    settings.setValue("NetCreateGeometry", saveGeometry());
    if ( ready ) {
        //settings.setValue("NetCreateShowDesc", showDescription->isChecked());
    };
    settings.endGroup();
}

/* public slots */
void CreateVirtNetwork::closeEvent(QCloseEvent *ev)
{
    if ( ev->type()==QEvent::Close ) {
        QString key = objectName();
        QString msg = QString("'<b>%1</b>' network editor closed.")
                .arg(task.object);
        sendMsg(msg);
        emit finished(key);
    };
}
void CreateVirtNetwork::readCapabilities()
{
    ready = true;
    if ( xmlFileName.isEmpty() ) {
        // create/define new VM
        qDebug()<<"new network";
    } else {
        // read for edit exist VM parameters
        QFile *_xml =
                new QFile(this);
        _xml->setFileName(xmlFileName);
        //_xml->setAutoRemove(true);
        _xml->open(QIODevice::ReadOnly);
        xmlDesc.append(_xml->readAll().constData());
        _xml->close();
        _xml->deleteLater();
        advancedWdg = new CreateVirtNetwork_Adv(
                    this,
                    task.action);
        advancedWdg->readXmlDescData(xmlDesc);
        setCentralWidget(advancedWdg);
        setEnabled(true);
    };
    connect(centralWidget(), SIGNAL(newName(const QString&)),
            this, SLOT(setNewWindowTitle(const QString&)));
    readDataLists();
}
void CreateVirtNetwork::readDataLists()
{
    if ( ready ) {
    } else {
        QString msg = QString("Read Data in %1 failed.")
                .arg(objectName());
        sendMsg( msg );
        // to done()
        set_Result();
    };
}
bool CreateVirtNetwork::buildXMLDescription()
{
    this->setEnabled(false);
    QDomDocument doc;

    bool read = xml->open();
    if (read) xml->write(doc.toByteArray(4).data());
    xml->close();
    return true;
}
void CreateVirtNetwork::set_Result()
{
    if ( ready ) { //sender()==ok ) {
        if ( !buildXMLDescription() ) {
            this->setEnabled(true);
            return;
        };
        QString _xml = xml->fileName();
        QStringList data;
        data.append("New Network XML'ed");
        data.append(QString("to <a href='%1'>%1</a>").arg(_xml));
        QString msg = data.join(" ");
        sendMsg(msg);
        // if ( showDescription->isChecked() )
        //     QDesktopServices::openUrl(QUrl(_xml));
        task.args.path = _xml;
        emit addNewTask(task);
    };
    close();
}
void CreateVirtNetwork::setNewWindowTitle(const QString &_name)
{
    QString connName = task.srcConName;
    setWindowTitle(
                QString("Network Editor / <%1> in [%2]")
                .arg(_name).arg(connName));
}
void CreateVirtNetwork::sendMsg(QString &msg)
{
    QString time = QTime::currentTime().toString();
    QString title = QString("Connection '%1'").arg(task.srcConName);
    QString currMsg = QString(
    "<b>%1 %2:</b><br><font color='blue'><b>EVENT</b></font>: %3")
            .arg(time).arg(title).arg(msg);
    emit errorMsg(currMsg);
}
