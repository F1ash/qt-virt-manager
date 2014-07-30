#include "vm_viewer.h"

VM_Viewer::VM_Viewer(QWidget *parent, virConnect *conn, QString arg1, QString arg2) :
    QMainWindow(parent), jobConnect(conn), connName(arg1), domain(arg2)
{
    setMinimumSize(100, 100);
    setWindowTitle(QString("<%1> Virtual Machine").arg(domain));
    restoreGeometry( settings.value("LXCTermGeometry").toByteArray() );
    toolBar = new ViewerToolBar(this);
    addToolBar(toolBar);
    if ( jobConnect!=NULL ) {
        type = QString().fromUtf8(virConnectGetType(jobConnect));
    } else type.clear();
    if ( type.isEmpty() ) {
        QMessageBox::information(this, "VM Viewer", "Job empty.");
    } else if ( type.toLower()=="lxc" ) {
        // startnow = 1/yes
        viewer = new LXC_Viewer(1, this, jobConnect, domain);
        // from qtermwidget/main.cpp
        menuBar = new QMenuBar(this);
        actionsMenu = new QMenu("Actions", menuBar);
        menuBar->addMenu(actionsMenu);
        actionsMenu->addAction("Find..",
                               viewer,
                               SLOT(toggleShowSearchBar()),
                               QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F));
        this->setMenuBar(menuBar);
    } else if ( type.toLower()=="qemu" || type.toLower()=="xen" ) {
        //viewer = new VNC_Viewer(this);
    } else QMessageBox::information(this, "VM Viewer", QString("Not implemented type: %1").arg(type));
    if ( viewer!=NULL ) {
        setCentralWidget(viewer);
        connect(viewer,
                SIGNAL(errorMsg(QString&)),
                this,
                SLOT(receiveErrMsg(QString&)));
        connect(viewer,
                SIGNAL(jobFinished()),
                this,
                SLOT(closeViewer()));
    };
    this->show();
    VM_State = true;
}
VM_Viewer::~VM_Viewer()
{
    settings.setValue("LXCTermGeometry", saveGeometry());
    if ( viewer!=NULL ) {
        disconnect(viewer,
                   SIGNAL(errorMsg(QString&)),
                   this,
                   SLOT(receiveErrMsg(QString&)));
        disconnect(viewer,
                   SIGNAL(jobFinished()),
                   this,
                   SLOT(closeViewer()));
        delete viewer;
        viewer = 0;
    };
    if ( actionsMenu!=NULL ) {
        delete actionsMenu;
        actionsMenu = 0;
    };
    if ( menuBar!=NULL ) {
        delete menuBar;
        menuBar = 0;
    };
    delete toolBar;
    toolBar = 0;
    if ( jobConnect!=NULL ) {
        virConnectClose(jobConnect);
    };
    VM_State = false;
}

/* public slots */
bool VM_Viewer::isActive() const
{
    return VM_State;
}
void VM_Viewer::stopProcessing()
{
    /*
     * reserved for stop viewer job
     */
    QString msg = QString("'<b>%1</b>' terminal closed.").arg(domain);
    receiveErrMsg(msg);
}

/* private slots */
void VM_Viewer::closeEvent(QCloseEvent *ev)
{
    if ( ev->type()==QEvent::Close ) {
        QString msg = QString("'<b>%1</b>' terminal hidden.").arg(domain);
        receiveErrMsg(msg);
        ev->accept();
    }
}
void VM_Viewer::closeViewer()
{
    qDebug()<<domain<<"Job Finished";
    QString msg = QString("'<b>%1</b>' terminal closed.").arg(domain);
    receiveErrMsg(msg);
    emit finished();
}
void VM_Viewer::receiveErrMsg(QString &msg)
{
    QString time = QTime::currentTime().toString();
    QString title = QString("Connect '%1'").arg(connName);
    QString errMsg = QString("<b>%1 %2:</b><br>%3").arg(time).arg(title).arg(msg);
    emit errorMsg(errMsg);
}
