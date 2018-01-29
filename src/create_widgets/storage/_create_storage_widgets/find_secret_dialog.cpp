#include "find_secret_dialog.h"

FindSecretDialog::FindSecretDialog(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    QDialog(parent), ptr_ConnPtr(connPtrPtr)
{
    setModal(true);
    setWindowTitle(tr("FindSecretDialog"));
    settings.beginGroup("FindSecretDialog");
    restoreGeometry(
                settings.value("Geometry").toByteArray());
    settings.endGroup();
    secrets = new VirtSecretControl(this);
    secrets->setUsageInSoftTouched(false);
    secrets->setCurrentWorkConnect(ptr_ConnPtr);
    hlpThread = new SecretControlThread(this);
    connect(hlpThread, SIGNAL(resultData(Result)),
            this, SLOT(hlpThreadResult(Result)));
    listLayout = new QHBoxLayout(this);
    listLayout->addWidget(secrets);
    listWidget = new QWidget(this);
    listWidget->setLayout(listLayout);

    chooseSecret = new QPushButton(
                QIcon::fromTheme("dialog-ok"), tr("Choose Secret"), this);
    cancel = new QPushButton(
                QIcon::fromTheme("dialog-cancel"), tr("Cancel"), this);
    connect(chooseSecret, SIGNAL(clicked()),
            this, SLOT(set_Result()));
    connect(cancel, SIGNAL(clicked()),
            this, SLOT(set_Result()));
    buttonLayout = new QHBoxLayout(this);
    buttonLayout->addWidget(chooseSecret);
    buttonLayout->addWidget(cancel);
    buttons = new QWidget(this);
    buttons->setLayout(buttonLayout);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(listWidget);
    commonLayout->addWidget(buttons);
    setLayout(commonLayout);
    connect(secrets, SIGNAL(entityMsg(const QString&)),
            this, SLOT(showMsg(const QString&)));
    connect(secrets, SIGNAL(addNewTask(TASK*)),
            this, SLOT(execAction(TASK*)));

    setSecretList();
}

/* public slots */
FSD_Result FindSecretDialog::getResult() const
{
    FSD_Result res;
    res.usage = secrets->getCurrentSecUsage();
    res.uuid  = secrets->getCurrentSecUUID();
    res.type  = secrets->getCurrentSecType();
    return res;
}
void FindSecretDialog::showMsg(const QString &msg)
{
    QMessageBox::information(
                this,
                tr("FindSecretDialog"),
                msg,
                QMessageBox::Ok);
}

/* private slots */
void FindSecretDialog::setSecretList()
{
    TASK _task;
    _task.type          = VIRT_ENTITY::VIRT_SECRET;
    _task.srcConnPtr    = ptr_ConnPtr;
    _task.action        = Actions::GET_ALL_ENTITY_STATE;
    hlpThread->execAction(0, _task);
}
void FindSecretDialog::set_Result()
{
    settings.beginGroup("FindSecretDialog");
    settings.setValue( "Geometry", saveGeometry());
    settings.endGroup();
    done( (sender()==chooseSecret)?
              QDialog::Accepted :
              QDialog::Rejected);
    //qDebug()<<"done";
}
void FindSecretDialog::execAction(TASK *_task)
{
    hlpThread->execAction(0, *_task);
}
void FindSecretDialog::hlpThreadResult(Result data)
{
    secrets->resultReceiver(&data);
    secrets->setEnabled(true);
}
