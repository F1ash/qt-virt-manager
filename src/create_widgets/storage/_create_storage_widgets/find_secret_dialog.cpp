#include "find_secret_dialog.h"

FindSecretDialog::FindSecretDialog(QWidget *parent, virConnectPtr *connPtr) :
    QDialog(parent), currConnPtr(connPtr)
{
    setModal(true);
    setWindowTitle("FindSecretDialog");
    secrets = new VirtSecretControl(this);
    virtSecretThread = new SecretControlThread(this);
    connect(virtSecretThread, SIGNAL(resultData(Result)),
            secrets, SLOT(resultReceiver(Result)));
    listLayout = new QHBoxLayout(this);
    listLayout->addWidget(secrets);
    listWidget = new QWidget(this);
    listWidget->setLayout(listLayout);

    chooseSecret = new QPushButton(
                QIcon::fromTheme("dialog-ok"), "Choose Secret", this);
    cancel = new QPushButton(
                QIcon::fromTheme("dialog-cancel"), "Cancel", this);
    connect(chooseSecret, SIGNAL(clicked()), this, SLOT(set_Result()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(set_Result()));
    buttonLayout = new QHBoxLayout(this);
    buttonLayout->addWidget(chooseSecret);
    buttonLayout->addWidget(cancel);
    buttons = new QWidget(this);
    buttons->setLayout(buttonLayout);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(listWidget);
    commonLayout->addWidget(buttons);
    setLayout(commonLayout);
    connect(secrets, SIGNAL(entityMsg(QString&)),
            this, SLOT(showMsg(QString&)));
    connect(secrets, SIGNAL(addNewTask(TASK)),
            this, SLOT(execAction(TASK)));

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
void FindSecretDialog::showMsg(QString &msg)
{
    QMessageBox::information(
                this,
                "FindSecretDialog",
                msg,
                QMessageBox::Ok);
}

/* private slots */
void FindSecretDialog::setSecretList()
{
    TASK _task;
    _task.type          = "secret";
    _task.srcConnPtr    = currConnPtr;
    _task.action        = GET_ALL_ENTITY_STATE;
    virtSecretThread->execAction(0, _task);
}
void FindSecretDialog::set_Result()
{
    done( (sender()==chooseSecret)?
              QDialog::Accepted :
              QDialog::Rejected);
    //qDebug()<<"done";
}
void FindSecretDialog::execAction(TASK _task)
{
    virtSecretThread->execAction(0, _task);
}
