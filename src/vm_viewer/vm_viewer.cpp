#include "vm_viewer.h"

VM_Viewer::VM_Viewer(QWidget *parent, virConnect *conn, QString str) :
    QMainWindow(parent), jobConnect(conn), domain(str)
{
    setMinimumSize(100, 100);
    setWindowTitle(QString("<%1> Virtual Machine").arg(domain));
    toolBar = new ViewerToolBar(this);
    addToolBar(toolBar);
    if ( jobConnect!=NULL ) {
        type = QString().fromUtf8(virConnectGetType(jobConnect));
    } else type.clear();
    if ( type.isEmpty() ) {
        QMessageBox::information(this, "VM Viewer", "Job empty.");
    } else if ( type.toLower()=="lxc" ) {
        viewer = new LXC_Viewer(this, jobConnect, domain);
    } else if ( type.toLower()=="qemu" || type.toLower()=="xen" ) {
        //viewer = new VNC_Viewer(this);
    } else QMessageBox::information(this, "VM Viewer", QString("Not implemented type: %1").arg(type));
    if ( viewer!=NULL ) setCentralWidget(viewer);
    this->show();
    VM_State = true;
}
VM_Viewer::~VM_Viewer()
{
    if ( viewer!=NULL ) {
        delete viewer;
        viewer = 0;
    };
    delete toolBar;
    toolBar = 0;
    if ( jobConnect!=NULL ) {
        virConnectClose(jobConnect);
    };
    VM_State = false;
    emit finished();
}

/* public slots */
bool VM_Viewer::isActive() const
{
    return VM_State;
}
void VM_Viewer::stopProcessing()
{
    close();
}

/* private slots */
void VM_Viewer::closeEvent(QCloseEvent *ev)
{
    deleteLater();
}
